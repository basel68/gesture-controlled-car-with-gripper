
#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/spi.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "Nrf.h"
#include "car.h"
#include "servo.h"
#define MISO_PIN_NUMBER 4
#define MOSI_PIN_NUMBER 7
#define SCK_PIN_NUMBER 6
#define CE_PIN_NUMBER 21
#define CSN_PIN_NUMBER 5
#define led 25



typedef struct
{
    uint8_t car_direction;
    uint8_t crane_direction;
} data_t;


typedef enum {
    CAR_FORWARD,
    CAR_BACKWARD,
    CAR_STOP
} CarState_t;


typedef enum {
    CRANE_UP,
    CRANE_DOWN
} CraneVerticalState_t;

typedef enum {
    CRANE_OPEN,
    CRANE_CLOSE
} CraneGripState_t;


typedef enum {
    CAR_MOVE_FORWARD,
    CAR_MOVE_BACKWARD,
    CAR_BRAKE
    
} CarEvent_t;

typedef enum {
    CRANE_LIFT_UP,
    CRANE_LOWER_DOWN
} CraneVerticalEvent_t;

typedef enum {
    CRANE_OPEN_GRIP,
    CRANE_CLOSE_GRIP
} CraneGripEvent_t;


typedef struct {
    CarState_t state;
} CarSystem_t;

typedef struct {
    CraneVerticalState_t verticalState;
    CraneGripState_t gripState;
} CraneSystem_t;


typedef struct {
    CarSystem_t carSystem;
    CraneSystem_t craneSystem;
} System_t;
typedef enum{
    TRANSITION,
    HANDLED,    
    IGNORED
}event_status_t;


void System_init(System_t *system);
event_status_t CarSystem_StateMachine(CarSystem_t *carSystem, CarEvent_t event);
event_status_t CraneVerticalMovement_StateMachine(CraneSystem_t *craneSystem, CraneVerticalEvent_t verticalEvent);
event_status_t CraneGrip_StateMachine(CraneSystem_t *craneSystem, CraneGripEvent_t gripEvent);

#endif // MAIN_H_

