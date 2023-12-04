#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "Nrf.h"
#include "car.h"
#define MISO_PIN_NUMBER 4
#define MOSI_PIN_NUMBER 7
#define SCK_PIN_NUMBER 6
#define CE_PIN_NUMBER 21
#define CSN_PIN_NUMBER 5
#define led 25
// Your NRF pins configuration based on your actual wiring
nrf_pin_t nrf_pins = {
    .CSN = CSN_PIN_NUMBER,
    .CE = CE_PIN_NUMBER,
    .SCK = SCK_PIN_NUMBER,
    .MOSI = MOSI_PIN_NUMBER,
    .MISO = MISO_PIN_NUMBER};
typedef struct
{
    uint16_t xjoy;
    uint16_t yjoy;
    uint16_t ax;
    uint16_t ay;
    uint16_t az;
} data_t;

data_t receivedData;

uint8_t direction = 0;

int main()
{
    stdio_init_all();
    // Initialize the NRF device
    nrf_init(&nrf_pins);
    car_init();
    carStop();
    nrf_configure_as_receiver(&nrf_pins);

    gpio_init(led);
    gpio_set_dir(led, GPIO_OUT);

    // Implement a loop to keep checking for received data
    // while (true)
    // {
    //     if (nrf_data_ready(&nrf_pins))
    //     {

    //         nrf_receive_data(&receivedData, sizeof(receivedData), &nrf_pins);

    //         printf("Received data:\n");
    //         printf("X Joy: %d\n", receivedData.xjoy);
    //         printf("Y Joy: %d\n", receivedData.yjoy);
    //         // printf("AX: %d\n", receivedData.ax);
    //         // printf("AY: %d\n", receivedData.ay);
    //         // printf("AZ: %d\n", receivedData.az);
    //     }
    //     else
    //     {
    //         printf("No data available\n");
    //     }

    //     if ((receivedData.xjoy > 2000 && receivedData.xjoy < 2200) &&
    //         (receivedData.yjoy > 2000 && receivedData.yjoy < 2200))
    //     {
    //         printf("stop");
    //         // Stop
    //         carStop();
    //     }
    //     else if ((receivedData.xjoy >= 0 && receivedData.xjoy < 50) &&
    //              (receivedData.yjoy > 2000 && receivedData.yjoy < 2200))
    //     {
    //         printf("forward");
    //         // Move forward
    //         carForward();
    //     }
    //     else if ((receivedData.xjoy > 4000 && receivedData.xjoy < 4200) &&
    //              (receivedData.yjoy > 2000 && receivedData.yjoy < 2200))
    //     {
    //         // Move backward
    //         printf("backward");
    //         carBackward();
    //     }else{
    //         printf("stop");
    //         // Stop
    //         carStop();
    //     }
    // }

    while (1)
    {
        if (nrf_data_ready(&nrf_pins))
        {
            nrf_receive_data(&direction, sizeof(direction), &nrf_pins);
            nrf_clear_receive_buffer(&nrf_pins);
            printf("Received data:\n");
            printf("direction: %d\n", direction);
            // printf("AX: %d\n", receivedData.ax);
            // printf("AY: %d\n", receivedData.ay);
            // printf("AZ: %d\n", receivedData.az);
        }
        else
        {
            //  printf("No data available\n");
            continue;
        }

        if (direction == 1)
        {
            carForward();
            printf("forward\n");
        }
        else if (direction == 2)
        {
            carBackward();
            printf("backward\n");
        }
        else
        {
            carStop();
            printf("stop\n");
        }
    }
    return 0;
}
