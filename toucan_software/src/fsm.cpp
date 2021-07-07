#include "fsm.h"

bool grab_can()
{
    // shut off flapper
    pwm_start(FLAPPER_MOTOR, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

    // stop wheels
    pwm_start(LEFT_WHEEL_A, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    pwm_start(LEFT_WHEEL_B, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    pwm_start(RIGHT_WHEEL_A, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    pwm_start(RIGHT_WHEEL_B, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(50);
  
    // close claw
    pwm_start(CLAW_SERVO, MOTOR_FREQ, CLAW_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(50);

    // check reflectance
    if (analogRead(CLAW_SENSOR) < CAN_SENSING_THRESHOLD)
    {
        return true;
    }
    return false;
}

void check_state() 
{
    static enum {INITIALIZE, SEARCH, GRAB_CAN, STORE_CAN, ALIGN, DROPOFF} state = INITIALIZE;
    static uint32_t time_elapsed = 0;

    switch(state) 
    {
        case INITIALIZE:
        // start-up sequence / waiting for the robot to touch ground, use tape sensors for this
            break;
        case SEARCH:
        // has initialized, stored a can, or completed drop-off -> follow tape, flapper on
            break;

        case GRAB_CAN:
        // can has been sensed -> flapper off, stop driving, grab can
            if (grab_can())
            {
                state = STORE_CAN;
            }
            break;

        case STORE_CAN:
        // can grabbed -> store can in reservoir
        // need memory for which reservoir slot to use? -> use global variable or something
            break;

        case ALIGN:
        // reached return vehicle -> stop flapper, change driving somehow, line up next to it
            break;

        case DROPOFF:
        // has aligned to the return vehicle
        // if no servo -> drive forwards slowly to drop cans, sense when the robot has reached the end of the return vehicle
        // if servo -> open the reservoir
            break;
        
        default:
            state = INITIALIZE;
            break;
    }
}

