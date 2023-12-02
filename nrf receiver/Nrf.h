#ifndef NRF_H_
#define NRF_H_
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#define nrf_start_address 0x00
typedef struct{
uint8_t MISO;
uint8_t MOSI;
uint8_t CSN;
uint8_t CE;
uint8_t SCK;
}nrf_pin_t;
extern uint8_t rx_addr_p0[];
extern uint8_t tx_addr_p0[];
#define NRF_CMD_FLUSH_TX    0xE1
#define NRF_CMD_R_RX_PAYLOAD 0x61
#define NRF_CMD_R_REGISTER 0x00
#define dummmy_byte 0xFF
#define spi_port_0 spi0
#define rf_chan_2Gh 2
#define port0_rx_adr 3
// RF_SETUP register bit positions for nRF24L01+
#define RF_SETUP_RF_DR_LOW   5   // Data rate low bit
#define RF_SETUP_RF_DR_HIGH  3   // Data rate high bit
#define RF_SETUP_RF_PWR_LOW  1   // RF output power low bit
#define RF_SETUP_RF_PWR_HIGH 2   // RF output power high bit

// Data rate settings
#define RF_SETUP_DR_1MBPS    0   // 0000 - 1Mbps
#define RF_SETUP_DR_2MBPS    (1 << RF_SETUP_RF_DR_HIGH) // 1000 - 2Mbps
#define RF_SETUP_DR_250KBPS  (1 << RF_SETUP_RF_DR_LOW)  // 0010 - 250kbps

// RF output power settings
#define RF_SETUP_PWR_MINUS_18DBM (0x00 << RF_SETUP_RF_PWR_LOW) // XX00 - -18dBm
#define RF_SETUP_PWR_MINUS_12DBM (0x01 << RF_SETUP_RF_PWR_LOW) // XX01 - -12dBm
#define RF_SETUP_PWR_MINUS_6DBM  (0x02 << RF_SETUP_RF_PWR_LOW) // XX10 - -6dBm
#define RF_SETUP_PWR_0DBM        (0x03 << RF_SETUP_RF_PWR_LOW) // XX11 - 0dBm
//status register macros
#define NRF_STATUS_RX_DR_BIT   6  // Data Ready RX FIFO interrupt. Asserted when new data arrives RX FIFO.
#define NRF_STATUS_TX_DS_BIT   5  // Data Sent TX FIFO interrupt. Asserted when packet transmitted on TX.
#define NRF_STATUS_MAX_RT_BIT  4  // Maximum number of TX retries interrupt. Set when MAX retransmits reached.
// RX_P_NO bits are 3 to 1
#define NRF_STATUS_TX_FULL_BIT 0  // TX FIFO full flag. 1: TX FIFO full. 0: Available locations in TX FIFO.


#define ENAA_p0 0

// CONFIG register bit positions
#define CONFIG_MASK_RX_DR    6
#define CONFIG_MASK_TX_DS    5
#define CONFIG_MASK_MAX_RT   4
#define CONFIG_EN_CRC        3
#define CONFIG_CRCO          2
#define CONFIG_PWR_UP        1
#define CONFIG_PRIM_RX       0

// Register map for nRF24L01+
#define NRF_CONFIG      0x00  // Configuration Register
#define NRF_EN_AA       0x01  // Enable 'Auto Acknowledgment' Function
#define NRF_EN_RXADDR   0x02  // Enable RX Addresses
#define NRF_SETUP_AW    0x03  // Setup of Address Widths (3, 4, 5 bytes)
#define NRF_SETUP_RETR  0x04  // Setup of Automatic Retransmission
#define NRF_RF_CH       0x05  // RF Channel
#define NRF_RF_SETUP    0x06  // RF Setup Register
#define NRF_STATUS      0x07  // Status Register
#define NRF_OBSERVE_TX  0x08  // Transmit observe register
#define NRF_CD          0x09  // Carrier Detect (or RPD)
#define NRF_RX_ADDR_P0  0x0A  // Receive address data pipe 0
#define NRF_RX_ADDR_P1  0x0B  // Receive address data pipe 1
#define NRF_RX_ADDR_P2  0x0C  // Receive address data pipe 2
#define NRF_RX_ADDR_P3  0x0D  // Receive address data pipe 3
#define NRF_RX_ADDR_P4  0x0E  // Receive address data pipe 4
#define NRF_RX_ADDR_P5  0x0F  // Receive address data pipe 5
#define NRF_TX_ADDR     0x10  // Transmit address
#define NRF_RX_PW_P0    0x11  // Number of bytes in RX payload in data pipe 0
#define NRF_RX_PW_P1    0x12  // Number of bytes in RX payload in data pipe 1
#define NRF_RX_PW_P2    0x13  // Number of bytes in RX payload in data pipe 2
#define NRF_RX_PW_P3    0x14  // Number of bytes in RX payload in data pipe 3
#define NRF_RX_PW_P4    0x15  // Number of bytes in RX payload in data pipe 4
#define NRF_RX_PW_P5    0x16  // Number of bytes in RX payload in data pipe 5
#define NRF_FIFO_STATUS 0x17  // FIFO Status Register
#define NRF_DYNPD       0x1C  // Enable dynamic payload length
#define NRF_FEATURE     0x1D  // Additional features register

#define NRF_CMD_W_TX_PAYLOAD 0xA0


#define EN_DPL 2

#define DPL_P0 0
typedef struct {
    uint8_t CONFIG;       // Configuration Register
    uint8_t EN_AA;        // Enable 'Auto Acknowledgment' Function
    uint8_t EN_RXADDR;    // Enable RX Addresses
    uint8_t SETUP_AW;     // Setup of Address Widths
    uint8_t SETUP_RETR;   // Setup of Automatic Retransmission
    uint8_t RF_CH;        // RF Channel
    uint8_t RF_SETUP;     // RF Setup Register
    uint8_t STATUS;       // Status Register
    uint8_t OBSERVE_TX;   // Transmit observe register
    uint8_t CD;           // Carrier Detect
    uint8_t RX_ADDR_P0[5];// Receive address data pipe 0
    uint8_t RX_ADDR_P1[5];// Receive address data pipe 1
    uint8_t RX_ADDR_P2;   // Receive address data pipe 2
    uint8_t RX_ADDR_P3;   // Receive address data pipe 3
    uint8_t RX_ADDR_P4;   // Receive address data pipe 4
    uint8_t RX_ADDR_P5;   // Receive address data pipe 5
    uint8_t TX_ADDR[5];   // Transmit address
    uint8_t RX_PW_P0;     // Number of bytes in RX payload in data pipe 0
    uint8_t RX_PW_P1;     // Number of bytes in RX payload in data pipe 1
    uint8_t RX_PW_P2;     // Number of bytes in RX payload in data pipe 2
    uint8_t RX_PW_P3;     // Number of bytes in RX payload in data pipe 3
    uint8_t RX_PW_P4;     // Number of bytes in RX payload in data pipe 4
    uint8_t RX_PW_P5;     // Number of bytes in RX payload in data pipe 5
    uint8_t FIFO_STATUS;  // FIFO Status Register
    uint8_t DYNPD;        // Enable dynamic payload length
    uint8_t FEATURE;      // Additional features register
} nrf_reg_def_t;
#define RF_PWR_LOW 1 
#define RF_PWR_HIGH 2 
#define RF_DR_LOW 5
#define RF_DR_HIGH 3
#define nrf_reg ((nrf_reg_def_t*) nrf_start_address)
#define RF_PWR 1 // Bit position for RF_PWR's least significant bit
#define RF_PWR_MAX 0x03 // '11' for 0dBm output power
void nrf_init( nrf_pin_t *nrf_pins) ;

bool nrf_send_data(uint8_t *payload, size_t len, nrf_pin_t *nrf_pin);

bool nrf_receive_data(uint8_t *data, size_t len, nrf_pin_t *nrf_pin);
void nrf_configure_as_receiver( nrf_pin_t *nrf_pins) ;
void nrf_configure_as_transmitter( nrf_pin_t *nrf_pins);

bool nrf_transmission_successful(nrf_pin_t *nrf_pins);

bool nrf_data_ready(nrf_pin_t *nrf_pins);
void check_auto_ack(nrf_pin_t *nrf_pins);


void print_all_registers(nrf_pin_t *nrf_pins);




#endif