#include "fsm.h"

volatile int reservoir_state = 0;

void check_state()
{
    static enum { INITIALIZE,
                  SEARCH,
                  GRAB_CAN,
                  STORE_CAN,
                  ALIGN,
                  DROPOFF } state = INITIALIZE;
    static uint32_t time_elapsed = 0;

    switch (state)
    {
    case INITIALIZE:
        // start-up sequence / waiting for the robot to touch ground, use tape sensors for this
        break;
    case SEARCH:
        // has initialized, stored a can, or completed drop-off -> follow tape, flapper on
        break;

    case GRAB_CAN:
        // can has been sensed -> flapper off, stop driving, grab can
        break;

    case STORE_CAN:
        // can grabbed -> store can in reservoir
        // need memory for which reservoir slot to use? -> use global variable or something
        // TODO: Brian
        boolean can_stored = store_can();
        if (can_stored)
        {
            if (reservoir_state < 2)
            {
                reservoir_state++;
            }
            else
            {
                reservoir_state = 0;
            }
        }

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

boolean store_can()
{
    delay(1000);

    for (int i = ARM_DOWN; i < ARM_UP; i += 25)
    {
        pwm_start(ARM_SERVO, MOTOR_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(15);
    }

    delay(1000);

    for (int i = SWIVEL_ORIGIN; i > RESERVOIR_POSITIONS[reservoir_state]; i -= 50)
    {
        pwm_start(SWIVEL_SERVO, MOTOR_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(30);
    }

    delay(1000);

    pwm_start(CLAW_SERVO, MOTOR_FREQ, CLAW_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

    delay(1000);

    for (int i = RESERVOIR_POSITIONS[reservoir_state]; i < SWIVEL_ORIGIN; i += 50)
    {
        pwm_start(SWIVEL_SERVO, MOTOR_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(30);
    }

    delay(1000);

    pwm_start(CLAW_SERVO, MOTOR_FREQ, CLAW_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

    delay(1000);

    for (int i = ARM_UP; i > ARM_DOWN; i -= 25)
    {
        pwm_start(ARM_SERVO, MOTOR_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(15);
    }

    delay(1000);

    pwm_start(CLAW_SERVO, MOTOR_FREQ, CLAW_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

    delay(1000);

    return true;
}