#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"


#define MOTOR1_PIN 25
#define MOTOR2_PIN 15
#define MOTOR3_PIN 16
#define MOTOR4_PIN 17
#define ENABLE_A_PIN 18
#define ENABLE_B_PIN 19
#define PWM_TOP 255

//joystick pins
#define JOY_X_PIN 27 // ADC capable pin for X-axis
#define JOY_Y_PIN 26 // ADC capable pin for Y-axis


uint get_pwm_slice_number(uint gpio) {
    return pwm_gpio_to_slice_num(gpio);
}

void setup() {
    stdio_init_all();

    
    gpio_init(MOTOR1_PIN);
    gpio_set_dir(MOTOR1_PIN, GPIO_OUT);
    
    gpio_init(MOTOR2_PIN);
    gpio_set_dir(MOTOR2_PIN, GPIO_OUT);
    
    gpio_init(MOTOR3_PIN);
    gpio_set_dir(MOTOR3_PIN, GPIO_OUT);
    
    gpio_init(MOTOR4_PIN);
    gpio_set_dir(MOTOR4_PIN, GPIO_OUT);
    
    
    gpio_set_function(ENABLE_A_PIN, GPIO_FUNC_PWM);
    gpio_set_function(ENABLE_B_PIN, GPIO_FUNC_PWM);

    // Set PWM top value
    pwm_set_wrap(get_pwm_slice_number(ENABLE_A_PIN), PWM_TOP);
    pwm_set_wrap(get_pwm_slice_number(ENABLE_B_PIN), PWM_TOP);

    // Set PWM frequency 
    pwm_set_clkdiv(get_pwm_slice_number(ENABLE_A_PIN), 4.f);  
    pwm_set_clkdiv(get_pwm_slice_number(ENABLE_B_PIN), 4.f);  

    // Initialize ADC for joystick
    adc_init();
    adc_gpio_init(JOY_X_PIN);
    adc_gpio_init(JOY_Y_PIN);
}

void set_motor_speed(uint gpio, float duty_cycle) {
    uint slice_num = get_pwm_slice_number(gpio);
    pwm_set_enabled(slice_num, true);
    pwm_set_gpio_level(gpio, duty_cycle * (float)PWM_TOP);
}

int read_joystick_axis(uint gpio) {
    adc_select_input(gpio == JOY_X_PIN ? 0 : 1); 
    return adc_read(); 
}

int main() {
    setup();

    while (1) {
        int xValue = read_joystick_axis(JOY_X_PIN);
        int yValue = read_joystick_axis(JOY_Y_PIN);

        

       
        if (xValue==513 && yValue==514) { 
            gpio_put(MOTOR1_PIN, 0);
            gpio_put(MOTOR2_PIN, 0);
            gpio_put(MOTOR3_PIN, 0);
            gpio_put(MOTOR4_PIN, 0);
        } else if(xValue<10){ //forward
            gpio_put(MOTOR1_PIN, 1);
            gpio_put(MOTOR2_PIN, 0);
            gpio_put(MOTOR3_PIN, 1);
            gpio_put(MOTOR4_PIN, 0);
        } else { // Backward
            gpio_put(MOTOR1_PIN, 0);
            gpio_put(MOTOR2_PIN, 1);
            gpio_put(MOTOR3_PIN, 0);
            gpio_put(MOTOR4_PIN, 1);
        }

      set_motor_speed(ENABLE_A_PIN, 1.0f); // Half speed
      set_motor_speed(ENABLE_B_PIN, 1.0f); // Half speed

        
        sleep_ms(100);
    }

    return 0;
}