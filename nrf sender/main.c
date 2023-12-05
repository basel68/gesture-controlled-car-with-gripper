#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "Nrf.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#define MISO_PIN_NUMBER 16
#define MOSI_PIN_NUMBER 19
#define SCK_PIN_NUMBER 18
#define CE_PIN_NUMBER 20
#define CSN_PIN_NUMBER 17
// joystick pins
#define JOY_X_PIN 27 // ADC capable pin for X-axis
#define JOY_Y_PIN 26 // ADC capable pin for Y-axis
// MPU6050 I2C address
#define MPU6050_ADDR 0x68

#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define PWR_MGMT_1 0x6B

// I2C Port
#define I2C_PORT i2c0
// Your NRF pins configuration based on your actual wiring
nrf_pin_t nrf_pins = {
    .CSN = CSN_PIN_NUMBER,
    .CE = CE_PIN_NUMBER,
    .SCK = SCK_PIN_NUMBER,
    .MOSI = MOSI_PIN_NUMBER,
    .MISO = MISO_PIN_NUMBER};
int read_joystick_axis(uint gpio)
{
    adc_select_input(gpio == JOY_X_PIN ? 0 : 1);
    return adc_read();
}
void mpu6050_init()
{
    uint8_t buf[2];
    buf[0] = PWR_MGMT_1;
    buf[1] = 0;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);
}

int16_t read_mpu6050_register(uint8_t reg)
{
    uint8_t buf[2];
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);
    return (int16_t)(buf[0] << 8 | buf[1]);
}
void setup()
{
    stdio_init_all();
    // Initialize ADC for joystick
    adc_init();
    adc_gpio_init(JOY_X_PIN);
    adc_gpio_init(JOY_Y_PIN);

    // Initialize the NRF device
    nrf_init(&nrf_pins);
    // Initialize I2C
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);

    // Initialize MPU6050
    mpu6050_init();

    nrf_configure_as_transmitter(&nrf_pins);
}
// A test payload to send
typedef struct
{
    uint8_t car_direction;
    uint8_t crane_direction;
} data_t;

int main()
{
    
    data_t payload;
    setup();
    while (1)
    {
        int xValue = read_joystick_axis(JOY_X_PIN);
        int yValue = read_joystick_axis(JOY_Y_PIN);
        int16_t ax = read_mpu6050_register(ACCEL_XOUT_H);
        int16_t ay = read_mpu6050_register(ACCEL_YOUT_H);

      
        if(ax<-10000){//front
            payload.crane_direction=0;
        }
        else if(ax>10000)
        {//back
        payload.crane_direction=1;
        }
        else if(ay>9000){//right
        payload.crane_direction=2;
        } else if(ay<-9000){//left
        payload.crane_direction=3;
         }

        if ((xValue > 2000 && xValue < 2200) &&
            (yValue > 2000 && yValue < 2200))
        {
            payload.car_direction = 0;
        }
        else if ((xValue >= 0 && xValue < 50) &&
                 (yValue > 2000 && yValue < 2200))
        {
            payload.car_direction = 1;
        }
        else if ((xValue > 4000 && xValue < 4200) &&
                 (yValue > 2000 && yValue < 2200))
        {
            payload.car_direction = 2;
        }

        if (nrf_send_data(&payload, sizeof(payload), &nrf_pins))
        {
            printf("direction: %d\n", payload.car_direction);
            printf("direction: %d\n", payload.crane_direction);
            printf("sent successfully\n");
        }
        else
        {
            printf("Transmission failed\n");
        }
        sleep_ms(200);
    }

    return 0;
}
