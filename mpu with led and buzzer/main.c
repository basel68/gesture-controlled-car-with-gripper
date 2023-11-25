#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// MPU6050 I2C address
#define MPU6050_ADDR 0x68
#define LED_PIN 8 
#define BUZZER_PIN 9
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H 0x43
#define GYRO_YOUT_H 0x45
#define GYRO_ZOUT_H 0x47
#define PWR_MGMT_1 0x6B

// I2C Port
#define I2C_PORT i2c0

void mpu6050_init() {
    uint8_t buf[2];
    buf[0] = PWR_MGMT_1;
    buf[1] = 0;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);
}

int16_t read_mpu6050_register(uint8_t reg) {
    uint8_t buf[2];
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);
    return (int16_t) (buf[0] << 8 | buf[1]);
}

int main() {
    stdio_init_all();
    
    // Initialize I2C
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    // Initialize MPU6050
    mpu6050_init();
    

    while (1) {
        int16_t ax = read_mpu6050_register(ACCEL_XOUT_H);
        int16_t ay = read_mpu6050_register(ACCEL_YOUT_H);
        int16_t az = read_mpu6050_register(ACCEL_ZOUT_H);
        
        int16_t gx = read_mpu6050_register(GYRO_XOUT_H);
        int16_t gy = read_mpu6050_register(GYRO_YOUT_H);
        int16_t gz = read_mpu6050_register(GYRO_ZOUT_H);

        printf("Accel: X=%d, Y=%d, Z=%d\n", ax, ay, az);
        printf("Gyro: X=%d, Y=%d, Z=%d\n", gx, gy, gz);
        // Control LED based on accelerometer X value
        if (ax < -10000) {
            gpio_put(LED_PIN, 1);  // Turn on LED
        } else {
            gpio_put(LED_PIN, 0);  // Turn off LED
        }
         // Control buzzer based on going back alert
        if (ax > 300) {
            gpio_put(BUZZER_PIN, 1);  // Turn on buzzer
        } else {
            gpio_put(BUZZER_PIN, 0);  // Turn off buzzer
        }

        sleep_ms(1000);
    }

    return 0;
}