#include "Nrf.h"

static void nrf_read_register(uint8_t reg, uint8_t *data, size_t len, nrf_pin_t *nrf_pin) ;
static void nrf_write_register(uint8_t reg,  uint8_t *data, size_t len,nrf_pin_t *nrf_pin);
static void nrf_spi_init(  nrf_pin_t *nrf_pins);
static void nrf_gpio_init( nrf_pin_t *nrf_pins);
uint8_t tx_addr_p0[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7}; // Same as receiver's RX_ADDR_P0
uint8_t rx_addr_p0[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7}; // For auto-acknowledgment

void nrf_clear_transmit_buffer(nrf_pin_t *nrf_pin) {
    // Command to flush TX FIFO
    uint8_t flush_tx_cmd = NRF_CMD_FLUSH_TX;

    gpio_put(nrf_pin->CSN, 0); // CSN low to start the SPI transaction
    spi_write_blocking(spi_port_0, &flush_tx_cmd, 1); // Send the flush TX FIFO command
    gpio_put(nrf_pin->CSN, 1); // CSN high to end the SPI transaction

    // Additionally, clear the MAX_RT and TX_DS interrupt flags
    uint8_t status = (1 << NRF_STATUS_MAX_RT_BIT) | (1 << NRF_STATUS_TX_DS_BIT);
    nrf_write_register(NRF_STATUS, &status, 1, nrf_pin);
}
void nrf_clear_receive_buffer(nrf_pin_t *nrf_pin) {
    // Command to flush RX FIFO
    uint8_t flush_rx_cmd = NRF_CMD_FLUSH_RX;

    gpio_put(nrf_pin->CSN, 0); // CSN low to start the SPI transaction
    spi_write_blocking(spi_port_0, &flush_rx_cmd, 1); // Send the flush RX FIFO command
    gpio_put(nrf_pin->CSN, 1); // CSN high to end the SPI transaction

    // Additionally, clear the RX_DR interrupt flag
    nrf_clear_rx_dr(&nrf_pin);
}

bool nrf_receive_data(uint8_t *data, size_t len, nrf_pin_t *nrf_pin) {
    if (nrf_data_ready(nrf_pin)) {
        print_register("FIFO_STATUS", NRF_FIFO_STATUS, nrf_pin);
        // Read the payload data from the RX FIFO
        uint8_t cmd = NRF_CMD_R_RX_PAYLOAD;
        gpio_put(nrf_pin->CSN, 0); // CSN low to start the SPI transaction
        spi_write_blocking(spi_port_0, &cmd, 1); // Send the command to read RX payload
        spi_read_blocking(spi_port_0, 0xFF, data, len); // Read the payload into the data buffer
        printf("data:%s",data);
        gpio_put(nrf_pin->CSN, 1); // CSN high to end the SPI transaction
        nrf_clear_rx_dr(nrf_pin);

        return true; // Data received successfully
    }
    return false; // No data received
}
void nrf_clear_rx_dr(nrf_pin_t *nrf_pin) {
    uint8_t status;
    // Read the current STATUS register value
    nrf_read_register(NRF_STATUS, &status, 1, nrf_pin);

    // Set the RX_DR bit in the STATUS register to clear it
    status |= (1 << NRF_STATUS_RX_DR_BIT); // Set RX_DR bit to 1 to clear it
    nrf_write_register(NRF_STATUS, &status, 1, nrf_pin); // Write back to STATUS register

}
bool nrf_send_data(uint8_t *payload, size_t len, nrf_pin_t *nrf_pin) {
    printf("%d\n",len);
    if (len > 32) {
        len = 32;
    }
    
    // Write the payload data to the TX FIFO using the W_TX_PAYLOAD command
    uint8_t buffer[len + 1];
    buffer[0] = NRF_CMD_W_TX_PAYLOAD;
    for (size_t i = 0; i < len; i++) {
        buffer[i + 1] = payload[i];
    }

    gpio_put(nrf_pin->CSN, 0); // CSN low to start the SPI transaction
    spi_write_blocking(spi_port_0, buffer, len + 1);
    gpio_put(nrf_pin->CSN, 1); // CSN high to end the SPI transaction
    print_register("FIFO_STATUS", NRF_FIFO_STATUS, nrf_pin);
    // Pulse the CE pin to initiate transmission
    gpio_put(nrf_pin->CE, 1);
    sleep_us(15); // Minimum CE high time is 10us; 15us for a margin
    gpio_put(nrf_pin->CE, 0); // CE low to end the transmission pulse

    // Check the transmission status
    uint8_t status;
    nrf_read_register(NRF_STATUS, &status, 1, nrf_pin);

    // If MAX_RT is set, the transmission failed due to maximum retries.
    if (status & (1 << NRF_STATUS_MAX_RT_BIT)) {
        // Flush the TX FIFO in case of MAX_RT
        uint8_t flush_tx_cmd = NRF_CMD_FLUSH_TX;
        gpio_put(nrf_pin->CSN, 0);
        spi_write_blocking(spi_port_0, &flush_tx_cmd, 1);
        gpio_put(nrf_pin->CSN, 1);

        // Clear the MAX_RT interrupt flag
        uint8_t clear_flag = (1 << NRF_STATUS_MAX_RT_BIT);
        nrf_write_register(NRF_STATUS, &clear_flag, 1, nrf_pin);
        return false;
    }

    // If TX_DS is set, the transmission was successful.
    if (status & (1 << NRF_STATUS_TX_DS_BIT)) {
        // Clear the TX_DS interrupt flag
        uint8_t clear_flag = (1 << NRF_STATUS_TX_DS_BIT);
        nrf_write_register(NRF_STATUS, &clear_flag, 1, nrf_pin);
        return true;
    }

    // If neither MAX_RT nor TX_DS is set, the transmission is still in progress or has failed for an unspecified reason.
    
    return false;
}






static void nrf_write_register(uint8_t reg, uint8_t *data, size_t len,nrf_pin_t *nrf_pin) {
    uint8_t buffer[len + 1];
    buffer[0] = reg | 0x20;  // The write command is OR-ed with the register address.
    
    for (size_t i = 0; i < len; i++) {
        buffer[i + 1] = data[i];
    }

    gpio_put(nrf_pin->CSN, 0); // CSN low to start the SPI transaction
    spi_write_blocking(spi_port_0, buffer, len + 1);
    gpio_put(nrf_pin->CSN, 1); // CSN high to end the SPI transaction
}

void nrf_init(nrf_pin_t *nrf_pins) {
    //initialization of gpio pins
    nrf_gpio_init(nrf_pins);
    //initialization of spi port
    nrf_spi_init(nrf_pins);
    // Power up the nrf
    uint8_t config_value = 1 << CONFIG_PWR_UP;
    nrf_write_register(NRF_CONFIG, &config_value, 1, nrf_pins);

    // Wait for at least 1.5ms for the module to enter standby mode
    sleep_ms(2);
    //set the address width
    uint8_t setup_aw_value = 0x03; // 5-byte address
    nrf_write_register(NRF_SETUP_AW, &setup_aw_value, 1,nrf_pins);
    //set rf channel
    uint8_t rf_ch_value = 76;
    nrf_write_register(NRF_RF_CH, &rf_ch_value, 1,nrf_pins);
 // Setting data rate to high (2Mbps) and RF power to 0dBm
uint8_t rf_setup_value = RF_SETUP_DR_2MBPS | RF_SETUP_PWR_0DBM;
nrf_write_register(NRF_RF_SETUP, &rf_setup_value, 1, nrf_pins);


    nrf_setup_retransmission(nrf_pins, 0x08, 0x0F);
    //Enable address port0
    uint8_t en_rxaddr_value = 1 << ENAA_p0;
    nrf_write_register(NRF_EN_RXADDR, &en_rxaddr_value, 1,nrf_pins);
     //ENABLE Acknoledgment ADDRESS PORT 0
    uint8_t en_aa_value = 0;
    nrf_write_register(NRF_EN_AA, &en_aa_value, 1,nrf_pins);
    
    nrf_enable_dynamic_payload(nrf_pins);

     //setting rx port 0 address 
    nrf_write_register(NRF_RX_ADDR_P0, rx_addr_p0, sizeof(rx_addr_p0),nrf_pins);
     //setting transmitter address
    nrf_write_register(NRF_TX_ADDR, tx_addr_p0, sizeof(tx_addr_p0),nrf_pins);
    //  //power up the nrf
    // uint8_t config_value = 1 << CONFIG_PWR_UP;
    // nrf_write_register(NRF_CONFIG, &config_value, 1,nrf_pins);

    
}

void nrf_enable_dynamic_payload(nrf_pin_t *nrf_pins) {
    uint8_t feature_reg = 0;
    uint8_t dynpd_reg = 0;

    // Read the current FEATURE register value
    nrf_read_register(NRF_FEATURE, &feature_reg, 1, nrf_pins);
    // Enable dynamic payload length by setting the EN_DPL bit
    feature_reg |= (1 << EN_DPL);
    // Write the updated value back to the FEATURE register
    nrf_write_register(NRF_FEATURE, &feature_reg, 1, nrf_pins);

    // Read the current DYNPD register value
    nrf_read_register(NRF_DYNPD, &dynpd_reg, 1, nrf_pins);
    // Enable dynamic payload for data pipe 0 by setting the DPL_P0 bit
    dynpd_reg |= (1 << DPL_P0);
    // Write the updated value back to the DYNPD register
    nrf_write_register(NRF_DYNPD, &dynpd_reg, 1, nrf_pins);

   
}
static void nrf_spi_init(  nrf_pin_t *nrf_pins) {
   
    spi_init(spi_port_0, 1000000); //clock speed
    
    //SPI pins
    spi_set_format(spi_port_0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    spi_set_slave(spi_port_0, false); // We are the master
    gpio_set_function(nrf_pins->SCK, GPIO_FUNC_SPI);
    gpio_set_function(nrf_pins->MOSI, GPIO_FUNC_SPI);
    gpio_set_function(nrf_pins->MISO, GPIO_FUNC_SPI);

   
}
static void nrf_gpio_init(nrf_pin_t *nrf_pins) {
    // Initialize the SPI_CS (Chip Select) pin as an output
    gpio_init(nrf_pins->CSN);
    gpio_set_dir(nrf_pins->CSN, GPIO_OUT);
    gpio_put(nrf_pins->CSN, 1); // Set CSN high to start with the device deselected

    // Initialize the CE (Chip Enable) pin as an output
    gpio_init(nrf_pins->CE);
    gpio_set_dir(nrf_pins->CE, GPIO_OUT);
    gpio_put(nrf_pins->CE, 0); // Set CE low to start in standby mode

    // Initialize the SPI_MISO (Master In Slave Out) pin as an input
    gpio_init(nrf_pins->MISO);
    gpio_set_dir(nrf_pins->MISO, GPIO_IN);
    // Optionally enable the pull-up if needed for your setup
    //gpio_pull_up(nrf_pins->MISO);

    // Initialize the SPI_MOSI (Master Out Slave In) pin as an output
    gpio_init(nrf_pins->MOSI);
    gpio_set_dir(nrf_pins->MOSI, GPIO_OUT);

    // Initialize the SPI_SCK (Serial Clock) pin as an output
    gpio_init(nrf_pins->SCK);
    gpio_set_dir(nrf_pins->SCK, GPIO_OUT);

}



static void nrf_read_register(uint8_t reg, uint8_t *data, size_t len, nrf_pin_t *nrf_pin) {
    uint8_t cmd = NRF_CMD_R_REGISTER | (reg & 0x1F);  // Command to read the specified register
    uint8_t buffer[len + 1];
    buffer[0] = cmd;

    // Ensure CSN is low to start the SPI transaction
    gpio_put(nrf_pin->CSN, 0);

    // Send the read command
    spi_write_blocking(spi_port_0, buffer, 1);

    // Read data from the SPI interface
    spi_read_blocking(spi_port_0, dummmy_byte, data, len);

    // End the SPI transaction by setting CSN high
    gpio_put(nrf_pin->CSN, 1);
}



void nrf_configure_as_receiver( nrf_pin_t *nrf_pins) {
    // Set PRIM_RX bit to 1 to set as receiver
    uint8_t config_value;
    nrf_read_register(NRF_CONFIG, &config_value, 1,nrf_pins);
    config_value |= 1 << CONFIG_PRIM_RX;
    nrf_write_register(NRF_CONFIG, &config_value, 1,nrf_pins);

   

    // set the CE pin high to start listening
    gpio_put(nrf_pins->CE, 1);
}
void nrf_configure_as_transmitter( nrf_pin_t *nrf_pins) {
    // Clear PRIM_RX bit to 0 to set as transmitter
    uint8_t config_value;
    nrf_read_register(NRF_CONFIG, &config_value, 1,nrf_pins);
    config_value &= ~(1 << CONFIG_PRIM_RX);
    nrf_write_register(NRF_CONFIG, &config_value, 1,nrf_pins);

  

    // Set the CE pin low in transmitter mode
    gpio_put(nrf_pins->CE, 0);
}


bool nrf_transmission_successful(nrf_pin_t *nrf_pins) {
    uint8_t status;
  
    nrf_read_register(NRF_STATUS, &status, 1, nrf_pins);
   printf("Status: 0x%02X\n", status);
    // If MAX_RT is set, the transmission failed due to maximum retries.
    if (status & (1 << NRF_STATUS_MAX_RT_BIT)) {

        uint8_t flush_tx_cmd = NRF_CMD_FLUSH_TX;
        gpio_put(nrf_pins->CSN, 0);
        spi_write_blocking(spi_port_0, &flush_tx_cmd, 1);
        gpio_put(nrf_pins->CSN, 1);

        // Writing '1' to clear the MAX_RT interrupt flag
        uint8_t clear_flag = (1 << NRF_STATUS_MAX_RT_BIT);
        nrf_write_register(NRF_STATUS, &clear_flag, 1, nrf_pins);
        return false;
    }

    // If TX_DS is set, the transmission was successful.
    if (status & (1 << NRF_STATUS_TX_DS_BIT)) {
        // Writing '1' to clear the TX_DS interrupt flag
        uint8_t clear_flag = (1 << NRF_STATUS_TX_DS_BIT);
        nrf_write_register(NRF_STATUS, &clear_flag, 1, nrf_pins);
        return true;
    }

    // Otherwise, the transmission is still in progress or has failed for an unspecified reason.
    return false;
}

bool nrf_data_ready(nrf_pin_t *nrf_pins) {
    uint8_t status;
 
    nrf_read_register(NRF_STATUS, &status, 1, nrf_pins);
    printf("Status: 0x%02X\n", status);
    // If RX_DR is set, there is data ready to be read.
    return (status & (1 << NRF_STATUS_RX_DR_BIT)) != 0;
}





void check_auto_ack(nrf_pin_t *nrf_pins) {
    uint8_t en_aa_value = 0;
    nrf_read_register(NRF_EN_AA, &en_aa_value, 1, nrf_pins);
    printf("EN_AA Register: 0x%02X\n", en_aa_value);
    if (en_aa_value & (1)) {
        printf("Auto-ACK on Pipe 0 is enabled.\n");
    } else {
        printf("Auto-ACK on Pipe 0 is disabled.\n");
    }
}
// Function to print a single register
void print_register(const char* name, uint8_t reg, nrf_pin_t *nrf_pins) {
    uint8_t value = 0;
    nrf_read_register(reg, &value, 1, nrf_pins);
    printf("%s (0x%02X): 0x%02X\n", name, reg, value);
}

// Function to print all important registers
void print_all_registers(nrf_pin_t *nrf_pins) {
    print_register("CONFIG", NRF_CONFIG, nrf_pins);
    print_register("EN_AA", NRF_EN_AA, nrf_pins);
    print_register("EN_RXADDR", NRF_EN_RXADDR, nrf_pins);
    print_register("SETUP_AW", NRF_SETUP_AW, nrf_pins);
    print_register("SETUP_RETR", NRF_SETUP_RETR, nrf_pins);
    print_register("RF_CH", NRF_RF_CH, nrf_pins);
    print_register("RF_SETUP", NRF_RF_SETUP, nrf_pins);
    print_register("STATUS", NRF_STATUS, nrf_pins);
    // print_register("OBSERVE_TX", NRF_OBSERVE_TX, nrf_pins);
    print_register("RX_ADDR_P0", NRF_RX_ADDR_P0, nrf_pins);
    print_register("TX_ADDR", NRF_TX_ADDR, nrf_pins);
    print_register("FIFO_STATUS", NRF_FIFO_STATUS, nrf_pins);
}
void print_5byte_address(const char* register_name, uint8_t reg, nrf_pin_t *nrf_pin) {
    uint8_t addr[5];
    nrf_read_register(reg, addr, 5, nrf_pin); // Read 5 bytes from the specified register

    printf("%s: ", register_name);
    for (int i = 0; i < 5; i++) {
        printf("0x%02X", addr[i]);
        if (i < 4) {
            printf(":");
        }
    }
    printf("\n");
}
void nrf_setup_retransmission(nrf_pin_t *nrf_pins, uint8_t delay, uint8_t count) {
   

    uint8_t setup_retr_value = ((delay & 0x0F) << 4) | (count & 0x0F);
    nrf_write_register(NRF_SETUP_RETR, &setup_retr_value, 1, nrf_pins);
}
