#include "main.h"

static event_status_t CarForward_stateHandler(CarSystem_t *carSystem, CarEvent_t event);
static event_status_t CarBackward_stateHandler(CarSystem_t *carSystem, CarEvent_t event);
static event_status_t CarStop_stateHandler(CarSystem_t *carSystem, CarEvent_t event);
static event_status_t CraneUp_stateHandler(CraneSystem_t *craneSystem, CraneVerticalEvent_t event);
static event_status_t CraneDown_stateHandler(CraneSystem_t *craneSystem, CraneVerticalEvent_t event);
static event_status_t CraneOpen_stateHandler(CraneSystem_t *craneSystem, CraneGripEvent_t event);
static event_status_t CraneClose_stateHandler(CraneSystem_t *craneSystem, CraneGripEvent_t event);

void System_init(System_t *system){
    system->carSystem.state=CAR_STOP;
    system->craneSystem.gripState=CRANE_OPEN;
    system->craneSystem.verticalState=CRANE_UP;
    CarSystem_StateMachine(&(system->carSystem),CAR_BRAKE);
    CraneVerticalMovement_StateMachine(&(system->craneSystem),CRANE_LIFT_UP);
    CraneGrip_StateMachine(&(system->craneSystem),CRANE_OPEN_GRIP);
}
event_status_t CarSystem_StateMachine(CarSystem_t *carSystem, CarEvent_t event){
 switch(carSystem->state){
    case CAR_STOP:
        return CarStop_stateHandler(carSystem,event);
    case CAR_BACKWARD:
        return CarBackward_stateHandler(carSystem,event);
    case CAR_FORWARD:
        return CarForward_stateHandler(carSystem,event);  
    default:
        return IGNORED;   
 }
}
event_status_t CraneVerticalMovement_StateMachine(CraneSystem_t *craneSystem, CraneVerticalEvent_t verticalEvent) {
    switch (craneSystem->verticalState) {
        case CRANE_UP:
            return CraneUp_stateHandler(craneSystem, verticalEvent);
        case CRANE_DOWN:
            return CraneDown_stateHandler(craneSystem, verticalEvent);
        default:
            return IGNORED;
    }
}
event_status_t CraneGrip_StateMachine(CraneSystem_t *craneSystem, CraneGripEvent_t gripEvent) {
    switch (craneSystem->gripState) {
        case CRANE_OPEN:
            return CraneOpen_stateHandler(craneSystem, gripEvent);
        case CRANE_CLOSE:
            return CraneClose_stateHandler(craneSystem, gripEvent);
        default:
            return IGNORED;
    }
}


// Forward state handler for the car
static event_status_t  CarForward_stateHandler(CarSystem_t *carSystem, CarEvent_t event) {
    switch (event) {
        case CAR_MOVE_FORWARD:
            {carForward();
            return HANDLED;}
        case CAR_MOVE_BACKWARD:
            {carBackward();
            carSystem->state=CAR_BACKWARD;
            return TRANSITION;}
        case CAR_BRAKE:
          {  carStop();
            carSystem->state=CAR_STOP;
            return TRANSITION;}
        default:
            return IGNORED;
    }
}

// Backward state handler for the car
static event_status_t CarBackward_stateHandler(CarSystem_t *carSystem, CarEvent_t event) {
   switch (event) {
        case CAR_MOVE_FORWARD:
            {carForward();
            carSystem->state=CAR_FORWARD;
            return TRANSITION;}
        case CAR_MOVE_BACKWARD:
           { carBackward();
            return HANDLED;}
        case CAR_BRAKE:
           { carStop();
            carSystem->state=CAR_STOP;
            return TRANSITION;}
        default:
            return IGNORED;
    }
}

// Stop state handler for the car
static event_status_t CarStop_stateHandler(CarSystem_t *carSystem, CarEvent_t event) {
    switch (event) {
        case CAR_MOVE_FORWARD:
            {carForward();
            carSystem->state=CAR_FORWARD;
            return TRANSITION;}
        case CAR_MOVE_BACKWARD:
             { carBackward();
            carSystem->state=CAR_BACKWARD;
            return TRANSITION;}
        case CAR_BRAKE:
            {carStop();
            return HANDLED;}
        default:
            return IGNORED;
    }
    
}
// Up state handler for the crane
static event_status_t CraneUp_stateHandler(CraneSystem_t *craneSystem, CraneVerticalEvent_t event) {
     switch (event) {
        case CRANE_LIFT_UP:{
          Crane_UP();
          return HANDLED;
          }
        case CRANE_LOWER_DOWN:{
          Crane_DOWN();
          craneSystem->verticalState=CRANE_DOWN;
          return TRANSITION;
          }
        default:
            return IGNORED;
    }
}

// Down state handler for the crane
static event_status_t CraneDown_stateHandler(CraneSystem_t *craneSystem, CraneVerticalEvent_t event) {
    switch (event) {
        case CRANE_LIFT_UP:{
          Crane_UP();
          craneSystem->verticalState=CRANE_UP;
          return TRANSITION;
          }
        case CRANE_LOWER_DOWN:{
           Crane_DOWN();
           return HANDLED;
           }
        default:
            return IGNORED;
    }
    
}

// Open state handler for the crane
static event_status_t CraneOpen_stateHandler(CraneSystem_t *craneSystem, CraneGripEvent_t event) {
    switch (event) {
        case CRANE_OPEN_GRIP:{
            Gripper_open();
            return HANDLED;
            }
        case CRANE_CLOSE_GRIP:{
            Gripper_close();
            craneSystem->gripState=CRANE_CLOSE;
            return TRANSITION;
            }
        default:
            return IGNORED;
    }
   
}

// Close state handler for the crane
static event_status_t CraneClose_stateHandler(CraneSystem_t *craneSystem, CraneGripEvent_t event) {
    switch (event) {
      case CRANE_OPEN_GRIP:{
            Gripper_open();
            craneSystem->gripState=CRANE_OPEN;
            return TRANSITION;
            }
        case CRANE_CLOSE_GRIP:{
            Gripper_close();
            return HANDLED;
            }
        default:
            return IGNORED;
    }
    
}
