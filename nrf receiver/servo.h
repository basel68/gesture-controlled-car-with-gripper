#ifndef SERVO_H_
#define SERVO_H_
#include "pico/stdlib.h"
#include "hardware/pwm.h"



#define SERVO_PIN_V 20
#define SERVO_PIN_H 21


uint angle_to_pwm_duty_cycle(int angle, uint32_t wrap_value);

void set_servo_angle(uint servo_pin, int angle, uint32_t wrap_value);
void servo_init();
void servo_angle(uint8_t angle,char servo);













#endif