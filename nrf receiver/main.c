#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "Nrf.h"
#include "car.h"
#include "servo.h"
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
    uint8_t car_direction;
    uint8_t crane_direction;
} data_t;

data_t receivedData;



int main()
{
    stdio_init_all();
    // Initialize the NRF device
    nrf_init(&nrf_pins);
    car_init();
    carStop();
    servo_init();
    nrf_configure_as_receiver(&nrf_pins);

    gpio_init(led);
    gpio_set_dir(led, GPIO_OUT);



    while (1)
    {
        if (nrf_data_ready(&nrf_pins))
        {
            nrf_receive_data(&receivedData, sizeof(receivedData), &nrf_pins);
            nrf_clear_receive_buffer(&nrf_pins);
            printf("Received data:\n");
            printf("direction: %d\n", receivedData.car_direction);
            printf("direction: %d\n", receivedData.crane_direction);
            // printf("AX: %d\n", receivedData.ax);
            // printf("AY: %d\n", receivedData.ay);
            // printf("AZ: %d\n", receivedData.az);
        }
        else
        {
            //  printf("No data available\n");
            continue;
        }
        if(receivedData.crane_direction==0){
            servo_angle(90,'V');
        }else if(receivedData.crane_direction==1){
            servo_angle(0,'V');
        }
        else if(receivedData.crane_direction==2){
            servo_angle(0,'H');
        }else if(receivedData.crane_direction==3){
            servo_angle(90,'H');
        }

        if (receivedData.car_direction == 1)
        {
            carForward();
            printf("forward\n");
        }
        else if (receivedData.car_direction == 2)
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
