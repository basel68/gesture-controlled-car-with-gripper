#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"


#define MOTOR1_PIN 25
#define MOTOR2_PIN 15
#define MOTOR3_PIN 16
#define MOTOR4_PIN 17
#define ENABLE_A_PIN 18
#define ENABLE_B_PIN 19
#define PWM_TOP 255
void set_motor_speed(uint gpio, float duty_cycle);
uint get_pwm_slice_number(uint gpio);
void car_init();
void set_motor_speed(uint gpio, float duty_cycle);
void carForward();
void carBackward();
void carStop();