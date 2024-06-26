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

uint get_pwm_slice_number(uint gpio) {
    return pwm_gpio_to_slice_num(gpio);
}

void setup() {
    stdio_init_all();

    // Initializing all motor control and enable pins as outputs
    gpio_init(MOTOR1_PIN);
    gpio_set_dir(MOTOR1_PIN, GPIO_OUT);
    
    gpio_init(MOTOR2_PIN);
    gpio_set_dir(MOTOR2_PIN, GPIO_OUT);
    
    gpio_init(MOTOR3_PIN);
    gpio_set_dir(MOTOR3_PIN, GPIO_OUT);
    
    gpio_init(MOTOR4_PIN);
    gpio_set_dir(MOTOR4_PIN, GPIO_OUT);
    
  


    // Initializing PWM on enable pins
    gpio_set_function(ENABLE_A_PIN, GPIO_FUNC_PWM);
    gpio_set_function(ENABLE_B_PIN, GPIO_FUNC_PWM);
    // Set PWM top value
    pwm_set_wrap(get_pwm_slice_number(ENABLE_A_PIN), PWM_TOP);
    pwm_set_wrap(get_pwm_slice_number(ENABLE_B_PIN), PWM_TOP);
    // Set PWM frequency (adjust as needed)
    pwm_set_clkdiv(get_pwm_slice_number(ENABLE_A_PIN), 4.f); 
    pwm_set_clkdiv(get_pwm_slice_number(ENABLE_B_PIN), 4.f); 
}

void set_motor_speed(uint gpio, float duty_cycle) {
    uint slice_num = get_pwm_slice_number(gpio);
    pwm_set_enabled(slice_num, true);
    pwm_set_gpio_level(gpio, duty_cycle * (float)PWM_TOP);
}

int main() {
    setup();

    while (1) {
        // Set motor direction
        gpio_put(MOTOR1_PIN, 1);
        gpio_put(MOTOR2_PIN, 0);
        gpio_put(MOTOR3_PIN, 1);
        gpio_put(MOTOR4_PIN, 0);

        
        set_motor_speed(ENABLE_A_PIN, 1.0f); // Full speed
        set_motor_speed(ENABLE_B_PIN, 1.0f); // Full speed
        sleep_ms(2000);

       
        set_motor_speed(ENABLE_A_PIN, 0.5f); // Half speed
        set_motor_speed(ENABLE_B_PIN, 0.5f); // Half speed
        sleep_ms(2000);

        // Stop motors
        gpio_put(MOTOR1_PIN, 0);
        gpio_put(MOTOR2_PIN, 0);
        gpio_put(MOTOR3_PIN, 0);
        gpio_put(MOTOR4_PIN, 0);
        set_motor_speed(ENABLE_A_PIN, 0.0f); 
        set_motor_speed(ENABLE_B_PIN, 0.0f); 
        sleep_ms(1000);
    }

    return 0;
}



