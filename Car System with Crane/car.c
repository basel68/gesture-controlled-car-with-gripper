#include "Car.h"
#include "servo.h"
uint get_pwm_slice_number(uint gpio)
{
    return pwm_gpio_to_slice_num(gpio);
}
void set_motor_speed(uint gpio, float duty_cycle)
{
    uint slice_num = get_pwm_slice_number(gpio);
    pwm_set_enabled(slice_num, true);
    pwm_set_gpio_level(gpio, duty_cycle * (float)PWM_TOP);
}

void car_init()
{
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
    carStop();
}
void Crane_UP(){
    servo_angle(80,'V');
}
void Crane_DOWN(){
    servo_angle(0,'V');
}
void Gripper_open(){
    servo_angle(0,'H');
}
void Gripper_close(){
    servo_angle(90,'H');
}
void carForward()
{
    gpio_put(MOTOR1_PIN, 1);
    gpio_put(MOTOR2_PIN, 0);
    gpio_put(MOTOR3_PIN, 1);
    gpio_put(MOTOR4_PIN, 0);
    set_motor_speed(ENABLE_A_PIN, 1.0f); // Full speed
    set_motor_speed(ENABLE_B_PIN, 1.0f); // Full speed
}
void carBackward()
{
    gpio_put(MOTOR1_PIN, 0);
    gpio_put(MOTOR2_PIN, 1);
    gpio_put(MOTOR3_PIN, 0);
    gpio_put(MOTOR4_PIN, 1);
    set_motor_speed(ENABLE_A_PIN, 1.0f); // Full speed
    set_motor_speed(ENABLE_B_PIN, 1.0f); // Full speed
}
void carStop()
{
    gpio_put(MOTOR1_PIN, 0);
    gpio_put(MOTOR2_PIN, 0);
    gpio_put(MOTOR3_PIN, 0);
    gpio_put(MOTOR4_PIN, 0);
    set_motor_speed(ENABLE_A_PIN, 0.0f); // 0 speed
    set_motor_speed(ENABLE_B_PIN, 0.0f); // 0 speed
}
void carTurnRight() {
    // Stop left motors while right motors move forward
    gpio_put(MOTOR1_PIN, 0);
    gpio_put(MOTOR2_PIN, 0);
    gpio_put(MOTOR3_PIN, 1);
    gpio_put(MOTOR4_PIN, 0);
    set_motor_speed(ENABLE_A_PIN, 0.0f); // Left motors off
    set_motor_speed(ENABLE_B_PIN, 1.0f); // Right motors full speed
}

void carTurnLeft() {
    // Stop right motors while left motors move forward
    gpio_put(MOTOR1_PIN, 1);
    gpio_put(MOTOR2_PIN, 0);
    gpio_put(MOTOR3_PIN, 0);
    gpio_put(MOTOR4_PIN, 0);
    set_motor_speed(ENABLE_A_PIN, 1.0f); // Left motors full speed
    set_motor_speed(ENABLE_B_PIN, 0.0f); // Right motors off
}