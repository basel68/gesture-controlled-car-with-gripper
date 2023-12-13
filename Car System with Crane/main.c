#include "main.h"
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


data_t receivedData;
// Task functions
void controlCar(void *pvParameters) {
    while (1) {
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
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for task scheduling
    }
}
void controlCrane(void *pvParameters) {
        if(receivedData.crane_direction==0){
            Crane_UP();
        }else if(receivedData.crane_direction==1){
            Crane_DOWN();
        }
        else if(receivedData.crane_direction==2){
            Gripper_open();
        }else if(receivedData.crane_direction==3){
            Gripper_close();
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for task scheduling
}

void receiveDataTask(void *pvParameters) {
    while (1) {
         if (nrf_data_ready(&nrf_pins))
        {
            nrf_receive_data(&receivedData, sizeof(receivedData), &nrf_pins);
            printf("Received data:\n");
            printf("direction: %d\n", receivedData.car_direction);
            printf("direction: %d\n", receivedData.crane_direction);
            nrf_clear_receive_buffer(&nrf_pins);
            // printf("AX: %d\n", receivedData.ax);
            // printf("AY: %d\n", receivedData.ay);
            // printf("AZ: %d\n", receivedData.az);
        }
        else
        {
              printf("No data available\n");
            continue;
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for task scheduling
    }
}
void vAssertCalled(const char * const file, unsigned long line) {
    // Handle the assertion failure
    printf("Assertion failed: File %s, Line %lu\n", file, line);
    // Infinite loop to halt the program
    while(1) { /* Add breakpoint here for debugging */ }
}



int main()
{
    stdio_init_all();
    // Initialize the NRF device
    nrf_init(&nrf_pins);
    car_init();
    servo_init();
    nrf_configure_as_receiver(&nrf_pins);

    gpio_init(led);
    gpio_set_dir(led, GPIO_OUT);

    xTaskCreate(receiveDataTask, "Receive Data Task", 256, NULL, 1, NULL);
    xTaskCreate(controlCar, "Control Car", 256, NULL, 1, NULL);
    xTaskCreate(controlCrane, "Control Crane", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    for(;;);
    return 0;
}
//STATE MACHINE VERSION
// #include "main.h"

// data_t receivedData;

// nrf_pin_t nrf_pins = {
//     .CSN = CSN_PIN_NUMBER,
//     .CE = CE_PIN_NUMBER,
//     .SCK = SCK_PIN_NUMBER,
//     .MOSI = MOSI_PIN_NUMBER,
//     .MISO = MISO_PIN_NUMBER};

// int main()
// {
//     stdio_init_all();
//     System_t *system;
//     // Initialize the NRF device
//     nrf_init(&nrf_pins);
//     car_init();
//     carStop();
//     servo_init();
//     nrf_configure_as_receiver(&nrf_pins);

//     gpio_init(led);
//     gpio_set_dir(led, GPIO_OUT);
//     System_init(system);


//     CarEvent_t car_event;
//     CraneGripEvent_t grip_event;
//     CraneVerticalEvent_t crane_event;
//     while (1)
//     {
//         if (nrf_data_ready(&nrf_pins))
//         {
//             nrf_receive_data(&receivedData, sizeof(receivedData), &nrf_pins);
//             printf("Received data:\n");
//             printf("direction: %d\n", receivedData.car_direction);
//             printf("direction: %d\n", receivedData.crane_direction);
//             nrf_clear_receive_buffer(&nrf_pins);
//             // printf("AX: %d\n", receivedData.ax);
//             // printf("AY: %d\n", receivedData.ay);
//             // printf("AZ: %d\n", receivedData.az);
//         }
//         else
//         {
//               printf("No data available\n");
//             continue;
//         }
//         if(receivedData.crane_direction==0){
//             crane_event=CRANE_LIFT_UP;
//         }else if(receivedData.crane_direction==1){
//             crane_event=CRANE_LOWER_DOWN;
//         }
//         else if(receivedData.crane_direction==2){
//             grip_event=CRANE_OPEN_GRIP;
//         }else if(receivedData.crane_direction==3){
//             grip_event=CRANE_CLOSE_GRIP;
//         }

//         if (receivedData.car_direction == 1)
//         {
//             car_event=CAR_MOVE_FORWARD;
//             printf("forward\n");
//         }
//         else if (receivedData.car_direction == 2)
//         {
//             car_event=CAR_MOVE_BACKWARD;
//             printf("backward\n");
//         }
//         else
//         {
//             car_event=CAR_BRAKE;
//             printf("stop\n");
//         }
//         CarSystem_StateMachine(&(system->carSystem),car_event);
//         CraneVerticalMovement_StateMachine(&(system->craneSystem),crane_event);
//         CraneGrip_StateMachine(&(system->craneSystem),grip_event);
//     }
//     return 0;
// }
