#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 20


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

int main() {
    // Initialize GPIO 20 for PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);

    // Get the PWM slice number
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    //calculate the wrap value for 50Hz PWM
    // Assuming a system clock of 125 MHz, and a target frequency of 50Hz
    uint32_t clock_freq = 125000000;
    uint32_t pwm_freq = 50;
    uint32_t wrap_value = clock_freq / pwm_freq / 16 - 1;

    pwm_set_wrap(slice_num, wrap_value);
    pwm_set_clkdiv(slice_num, 16.0); //divider to achieve the correct PWM frequency

    while (1) {
        set_servo_angle(SERVO_PIN, 0, wrap_value); // 0 degrees
        sleep_ms(1000);
        set_servo_angle(SERVO_PIN, 90, wrap_value); // 90 degrees
        sleep_ms(1000);
        set_servo_angle(SERVO_PIN, 180, wrap_value); // 180 degrees
        sleep_ms(1000);

    }

    return 0;
}
