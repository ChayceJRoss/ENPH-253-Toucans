#include "fsm.h"

volatile int reservoir_state = 0;

void drive()
{
    // PID code here
}

bool search()
{
    drive();
    // start flapper
    pwm_start(FLAPPER_MOTOR, MOTOR_FREQ, DC_FREQ, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    if (analogRead(CLAW_SENSOR) < CAN_SENSING_THRESHOLD)
    {
        // shut off flapper
        pwm_start(FLAPPER_MOTOR, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

        // stop wheels
        pwm_start(LEFT_WHEEL_A, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        pwm_start(LEFT_WHEEL_B, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        pwm_start(RIGHT_WHEEL_A, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        pwm_start(RIGHT_WHEEL_B, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(50);

        return true;
    }
    return false;
}

bool grab_can()
{
    // close claw
    pwm_start(CLAW_SERVO, MOTOR_FREQ, CLAW_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(50);

    // check reflectance -> or change this to a switch reading
    if (analogRead(CLAW_SENSOR) < CAN_SENSING_THRESHOLD)
    {
        return true;
    }
    return false;
}

bool store_can()
{
    if (analogRead(CLAW_SENSOR) > CAN_SENSING_THRESHOLD)
    {
        reset_claw();
        return false;
    }
    delay(1000);

    for (int i = ARM_DOWN; i < ARM_UP; i += 25)
    {
        pwm_start(ARM_SERVO, MOTOR_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(15);
    }

    if (analogRead(CLAW_SENSOR) > CAN_SENSING_THRESHOLD)
    {
        reset_claw();
        return false;
    }
    delay(1000);

    for (int i = SWIVEL_ORIGIN; i > RESERVOIR_POSITIONS[reservoir_state]; i -= 50)
    {
        pwm_start(SWIVEL_SERVO, MOTOR_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(30);
    }

    if (analogRead(CLAW_SENSOR) > CAN_SENSING_THRESHOLD)
    {
        reset_claw();
        return false;
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

    if (reservoir_state < 2)
    {
        reservoir_state++;
    }
    else
    {
        reservoir_state = 0;
    }

    return true;
}

void reset_claw()
{
    pwm_start(SWIVEL_SERVO, MOTOR_FREQ, SWIVEL_ORIGIN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(500);
    pwm_start(CLAW_SERVO, MOTOR_FREQ, CLAW_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(500);
    pwm_start(ARM_SERVO, MOTOR_FREQ, ARM_DOWN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(500);
    pwm_start(CLAW_SERVO, MOTOR_FREQ, CLAW_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(500);
}

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
        state = SEARCH;

        break;

    case SEARCH:
        // has initialized, stored a can, or completed drop-off -> follow tape, flapper on

        if (search())
        {
            state = GRAB_CAN;
        }
        break;

    case GRAB_CAN:
        // can has been sensed -> grab can

        if (grab_can())
        {
            state = STORE_CAN;
        }
        break;

    case STORE_CAN:
        // store can -> search

        store_can();
        state = SEARCH;
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