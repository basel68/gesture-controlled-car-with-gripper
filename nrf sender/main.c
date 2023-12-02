#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "Nrf.h"
#define MISO_PIN_NUMBER 16
#define MOSI_PIN_NUMBER 19
#define SCK_PIN_NUMBER 18
#define CE_PIN_NUMBER 20
#define CSN_PIN_NUMBER 17
// Your NRF pins configuration based on your actual wiring
nrf_pin_t nrf_pins = {
    .CSN = CSN_PIN_NUMBER,
    .CE = CE_PIN_NUMBER,
    .SCK = SCK_PIN_NUMBER,
    .MOSI = MOSI_PIN_NUMBER,
    .MISO = MISO_PIN_NUMBER
};

// A test payload to send
uint8_t payload[] = "Hello, NRF!";

int main() {
    stdio_init_all();
    // Initialize the NRF device
    nrf_init(&nrf_pins);

   
    nrf_configure_as_transmitter(&nrf_pins);
while (1)
{
    // Print RX_ADDR_P0 address
    // print_5byte_address("RX_ADDR_P0", NRF_RX_ADDR_P0, &nrf_pins);

    // // Print TX_ADDR address
    // print_5byte_address("TX_ADDR", NRF_TX_ADDR, &nrf_pins);
    // print_all_registers(&nrf_pins);
    // check_auto_ack(&nrf_pins);
    // send the payload
   
    
    // Check for transmission complete status
    if  (nrf_send_data(payload, sizeof(payload),&nrf_pins)) {
    //  print_all_registers(&nrf_pins);

     printf("sent successfully\n");

    //  print_all_registers(&nrf_pins);
    } else {
       printf("Transmission failed\n");
    }
    sleep_ms(1000);
}
    
    return 0;
}
