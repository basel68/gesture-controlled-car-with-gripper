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
typedef struct{
uint8_t xjoy;
uint8_t yjoy;
uint16_t ax;
uint16_t ay;
uint16_t az;
}data_t;

 

int main() {
    stdio_init_all();
    // Initialize the NRF device
    nrf_init(&nrf_pins);

    nrf_configure_as_receiver(&nrf_pins);

    

    // Implement a loop to keep checking for received data
    while (true) {
        
        
        if (nrf_data_ready(&nrf_pins)) {
            data_t receivedData;
            nrf_receive_data(&receivedData, sizeof(receivedData), &nrf_pins);

            printf("Received data:\n");
            printf("X Joy: %d\n", receivedData.xjoy);
            printf("Y Joy: %d\n", receivedData.yjoy);
            printf("AX: %d\n", receivedData.ax);
            printf("AY: %d\n", receivedData.ay);
            printf("AZ: %d\n", receivedData.az);
        } else {
            printf("No data available\n");
        }
        sleep_ms(1000);
    }

 
    
    return 0;
}
