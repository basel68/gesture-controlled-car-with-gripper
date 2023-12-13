#include "servo.h"

const uint32_t clock_freq = 125000000;
const uint32_t pwm_freq = 50;
const uint32_t wrap_value = clock_freq / pwm_freq / 16 - 1;

uint angle_to_pwm_duty_cycle(int angle, uint32_t wrap_value) {
    const float min_duty = 0.025; // 2.5% duty cycle corresponds to 0 degrees
    const float max_duty = 0.125; // 12.5% duty cycle corresponds to 180 degrees

    float duty_cycle = min_duty + (angle / 180.0f) * (max_duty - min_duty);
    return (uint16_t)(duty_cycle * wrap_value);
}

void set_servo_angle(uint servo_pin, int angle, uint32_t wrap_value) {
    uint slice_num = pwm_gpio_to_slice_num(servo_pin);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(servo_pin), angle_to_pwm_duty_cycle(angle, wrap_value));
    pwm_set_enabled(slice_num, true);
}



void servo_init(){
 // Initialize GPIO 20 for PWM
    gpio_set_function(SERVO_PIN_H, GPIO_FUNC_PWM);
    gpio_set_function(SERVO_PIN_V, GPIO_FUNC_PWM);

    // Get the PWM slice number
    uint slice_num_V = pwm_gpio_to_slice_num(SERVO_PIN_V);
    uint slice_num_H = pwm_gpio_to_slice_num(SERVO_PIN_H);

    pwm_set_wrap(slice_num_V, wrap_value);
    pwm_set_wrap(slice_num_H, wrap_value);
    pwm_set_clkdiv(slice_num_V, 16.0); //divider to achieve the correct PWM frequency
    pwm_set_clkdiv(slice_num_H, 16.0);
}
void servo_angle(uint8_t angle,char servo){
    if(servo=='V'){
     set_servo_angle(SERVO_PIN_V, angle, wrap_value);
    }
    else{
     set_servo_angle(SERVO_PIN_H, angle, wrap_value);
    }
}