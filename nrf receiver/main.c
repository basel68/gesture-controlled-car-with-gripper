#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "Nrf.h"
#define MISO_PIN_NUMBER 4
#define MOSI_PIN_NUMBER 7
#define SCK_PIN_NUMBER 6
#define CE_PIN_NUMBER 21
#define CSN_PIN_NUMBER 5
// Your NRF pins configuration based on your actual wiring
nrf_pin_t nrf_pins = {
    .CSN = CSN_PIN_NUMBER,
    .CE = CE_PIN_NUMBER,
    .SCK = SCK_PIN_NUMBER,
    .MOSI = MOSI_PIN_NUMBER,
    .MISO = MISO_PIN_NUMBER
};


uint8_t buffer[32]; 

int main() {
    stdio_init_all();
    // Initialize the NRF device
    nrf_init(&nrf_pins);

    nrf_configure_as_receiver(&nrf_pins);

    

    // Implement a loop to keep checking for received data
    while (true) {
        
        // check_auto_ack(&nrf_pins);
        // Print RX_ADDR_P0 address
    // print_5byte_address("RX_ADDR_P0", NRF_RX_ADDR_P0, &nrf_pins);

    // // Print TX_ADDR address
    // print_5byte_address("TX_ADDR", NRF_TX_ADDR, &nrf_pins);
        if (nrf_data_ready(&nrf_pins)) {
            // print_all_registers(&nrf_pins);
            nrf_receive_data(buffer, sizeof(buffer),&nrf_pins);
            // print_all_registers(&nrf_pins);
            sleep_ms(1000);
            printf("Received data: %s\n", buffer);
             
        }
        else{
            printf("fifo empty\n" );
            
        }
        sleep_ms(1000);
    }

 
    
    return 0;
}
