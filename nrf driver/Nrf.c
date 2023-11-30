#include "Nrf.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"





void nrf_write_register(uint8_t reg, const uint8_t *data, size_t len) {
    uint8_t buffer[len + 1];
    buffer[0] = reg | 0x20;  // The write command is OR-ed with the register address.
    
    for (size_t i = 0; i < len; i++) {
        buffer[i + 1] = data[i];
    }

    gpio_put(nrf_pin.CSN, 0); // CSN low to start the SPI transaction
    spi_write_blocking(spi_port, buffer, len + 1);
    gpio_put(nrf_pin.CSN, 1); // CSN high to end the SPI transaction
}

void nrf_init(const nrf_pin_t *nrf_pins) {
    //initialization of gpio pins
    nrf_gpio_init(nrf_pins);
    //set the address width
    uint8_t setup_aw_value = 0x03; // 5-byte address
    nrf_write_register(NRF_SETUP_AW, &setup_aw_value, 1);
    //set rf channel
    uint8_t rf_ch_value = rf_chan_2Gh;
    nrf_write_register(NRF_RF_CH, &rf_ch_value, 1);
    // setting data rate to low and rf power
    uint8_t rf_setup_value = RF_SETUP_DR_250KBPS | RF_SETUP_PWR_MINUS_6DBM;
    nrf_write_register(NRF_RF_SETUP, &rf_setup_value, 1);
    //Enable address port0
    uint8_t en_rxaddr_value = 1 << ENAA_p0;
    nrf_write_register(NRF_EN_RXADDR, &en_rxaddr_value, 1);
    //ENABLE Acknoledgment ADDRESS PORT 0
    uint8_t en_aa_value = 1 << ENAA_p0;
    nrf_write_register(NRF_EN_AA, &en_aa_value, 1);
     //setting rx port 0 address 
    nrf_write_register(NRF_RX_ADDR_P0, rx_addr_p0, sizeof(rx_addr_p0));
     //setting transmitter address
    nrf_write_register(NRF_TX_ADDR, tx_addr_p0, sizeof(tx_addr_p0));
     //power up the nrf
    uint8_t config_value = 1 << CONFIG_PWR_UP;
    nrf_write_register(NRF_CONFIG, &config_value, 1);

    
}


void nrf_spi_init(spi_inst_t *spi_port, const nrf_pin_t *nrf_pins) {
    // Configure the SPI hardware
    spi_init(*spi_port, 1000000); // Set your desired SPI clock speed
    
    // Configure SPI pins
    spi_set_format(*spi_port, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    spi_set_slave(*spi_port, false); // We are the master
    gpio_set_function(nrf_pins->SCK, GPIO_FUNC_SPI);
    gpio_set_function(nrf_pins->MOSI, GPIO_FUNC_SPI);
    gpio_set_function(nrf_pins->MISO, GPIO_FUNC_SPI);

   
}

void nrf_gpio_init(const nrf_pin_t *nrf_pins) {
    // Initialize the SPI_CS (Chip Select) pin as an output
    gpio_init(nrf_pins->CSN);
    gpio_set_dir(nrf_pins->CSN, GPIO_OUT);
    
    // Initialize the CE (Chip Enable) pin as an output
    gpio_init(nrf_pins->CE);
    gpio_set_dir(nrf_pins->CE, GPIO_OUT);

    // Initialize the SPI_MISO (Master In Slave Out) pin as an input
    gpio_init(nrf_pins->MISO);
    gpio_set_dir(nrf_pins->MISO, GPIO_IN);

    // Initialize the SPI_MOSI (Master Out Slave In) pin as an output
    gpio_init(nrf_pins->MOSI);
    gpio_set_dir(nrf_pins->MOSI, GPIO_OUT);

    // Initialize the SPI_SCK (Serial Clock) pin as an output
    gpio_init(nrf_pins->SCK);
    gpio_set_dir(nrf_pins->SCK, GPIO_OUT);

    
}
void nrf_read_data(uint8_t *data, size_t len) {
    uint8_t cmd = NRF_CMD_R_RX_PAYLOAD;  // Command to read payload
    uint8_t buffer[len + 1];
    buffer[0] = cmd;

    // Ensure CSN is low to start the SPI transaction
    gpio_put(nrf_pin.CSN, 0);

    // Send the read command
    spi_write_blocking(spi_port, buffer, 1);

    // Read data from the SPI interface
    spi_read_blocking(spi_port, data, len);

    // End the SPI transaction by setting CSN high
    gpio_put(nrf_pin.CSN, 1);
}

