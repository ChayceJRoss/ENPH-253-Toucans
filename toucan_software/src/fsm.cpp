#include "fsm.h"
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

volatile int init_time_sensed = 0;

void drive()
{
    // PID code here
}

bool search()
{
    delay(2000);
    return true;
    drive();
    // start flapper
    pwm_start(FLAPPER_MOTOR, MOTOR_FREQ, DC_FREQ, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    bool can_sensed = false;
    if(analogRead(CLAW_SENSOR) < CAN_SENSING_THRESHOLD){
        if (init_time_sensed == 0)
        {
            init_time_sensed = millis();
        }
        can_sensed = true;
    }
    else
    {
        init_time_sensed = 0;
        can_sensed = false;
    }
    if (analogRead(CLAW_SENSOR) < CAN_SENSING_THRESHOLD && can_sensed && (millis() - init_time_sensed > TIME_TO_GRAB_CAN_THRESHOLD))
    {
        // shut off flapper
        pwm_start(FLAPPER_MOTOR, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

        // stop wheels
        pwm_start(LEFT_WHEEL_A, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        pwm_start(LEFT_WHEEL_B, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        pwm_start(RIGHT_WHEEL_A, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        pwm_start(RIGHT_WHEEL_B, MOTOR_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        // delay(50);
        
        return true;
    }
    return false;
}

bool grab_can()
{ 
    // close claw
    pwm_start(CLAW_SERVO, MOTOR_FREQ, CLAW_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(200);
    pwm_start(CLAW_SERVO, MOTOR_FREQ, CLAW_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(200);

    // check reflectance -> or change this to a switch reading
    if (analogRead(CLAW_SENSOR) < CAN_SENSING_THRESHOLD)
    {
        return true;
    }
    return false;
}

void check_state()
{   
    static enum {INITIALIZE, SEARCH, GRAB_CAN, STORE_CAN, ALIGN, DROPOFF} state = INITIALIZE;
    display.println("state: ");
    display.println(state);
    display.println("reflectance: ");
    display.println(analogRead(CLAW_SENSOR));
    switch(state)
    {
        case INITIALIZE:
        // start-up sequence / waiting for the robot to touch ground, use tape sensors for this
            delay(2000);
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
