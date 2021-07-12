#include "fsm.h"
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

volatile int init_time_sensed = 0;
volatile int reservoir_state = 0;

volatile bool at_dropoff = false;
volatile int reflectance;

volatile int p = 0;
volatile int d = 0;
volatile int i = 0;
volatile int g = 0;
volatile int error = 0;
volatile int lasterror = 0;

void turn_wheels(int g)
{
    if (error == 0)
    {
        pwm_start(LEFT_WHEEL_A, SERVO_FREQ, CRUISING_SPEED, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(LEFT_WHEEL_B, SERVO_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);

        pwm_start(RIGHT_WHEEL_A, SERVO_FREQ, CRUISING_SPEED * RW_ADJUSTMENT_FACTOR, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(RIGHT_WHEEL_B, SERVO_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
    }
    else if (error < 0)
    {
        pwm_start(LEFT_WHEEL_A, SERVO_FREQ, CRUISING_SPEED, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(LEFT_WHEEL_B, SERVO_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);

        pwm_start(RIGHT_WHEEL_A, SERVO_FREQ, CRUISING_SPEED * RW_ADJUSTMENT_FACTOR / g, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(RIGHT_WHEEL_B, SERVO_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
    }
    else if (error > 0)
    {
        pwm_start(LEFT_WHEEL_A, SERVO_FREQ, CRUISING_SPEED / g, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(LEFT_WHEEL_B, SERVO_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);

        pwm_start(RIGHT_WHEEL_A, SERVO_FREQ, CRUISING_SPEED * RW_ADJUSTMENT_FACTOR, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(RIGHT_WHEEL_B, SERVO_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
    }
}

void drive()
{
    static enum { LEFT,
                  RIGHT } prev_state = LEFT;

    int left_reading = analogRead(LEFT_TAPE_SENSOR);
    int right_reading = analogRead(RIGHT_TAPE_SENSOR);

    display.print("Left: ");
    display.println(left_reading);
    display.print("Right: ");
    display.println(right_reading);

    // int set = 0;

    int kp = analogRead(P_POT) / 10;
    int kd = analogRead(D_POT) / 10;
    int ki = analogRead(I_POT) / 10;

    // int error;

    if (left_reading > BW_THRES && right_reading > BW_THRES)
    {
        error = 0;
    }
    else if (left_reading < BW_THRES && right_reading > BW_THRES)
    {
        error = -1;
        prev_state = LEFT;
    }
    else if (left_reading > BW_THRES && right_reading < BW_THRES)
    {
        error = 1;
        prev_state = RIGHT;
    }
    else if (left_reading < BW_THRES && right_reading < BW_THRES)
    {
        if (prev_state == LEFT)
        {
            error = -5;
        }
        else
        {
            error = 5;
        }
    }

    p = kp * error;
    d = kd * (error - lasterror);
    i = ki * error + i;

    if (i > MAX_INTEGRATOR_VALUE)
    {
        i = MAX_INTEGRATOR_VALUE;
    }
    else if (i < MAX_INTEGRATOR_VALUE)
    {
        i = MAX_INTEGRATOR_VALUE;
    }

    g = p + i + d;
    if (g < 0)
    {
        g = g * -1;
    }
    if (g > 4095)
    {
        g = 4095;
    }
    turn_wheels(g);
    lasterror = error;

    display.print("P: ");
    display.println(analogRead(P_POT));
    display.print("I: ");
    display.println(analogRead(I_POT));
    display.print("D: ");
    display.println(analogRead(D_POT));
    display.print("G: ");
    display.println(g);
    display.print("Error: ");
    display.println(error);

    delay(100);
}

bool search()
{
    drive();
    // start flapper
    // pwm_start(FLAPPER_MOTOR, SERVO_FREQ, DC_FREQ, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    // bool can_sensed = false;
    // if(analogRead(CLAW_SENSOR) < CAN_SENSING_THRESHOLD){
    //     if (init_time_sensed == 0)
    //     {
    //         init_time_sensed = millis();
    //     }
    //     can_sensed = true;
    // }
    // else
    // {
    //     init_time_sensed = 0;
    //     can_sensed = false;
    // }
    // if (analogRead(CLAW_SENSOR) < CAN_SENSING_THRESHOLD && can_sensed && (millis() - init_time_sensed > TIME_TO_GRAB_CAN_THRESHOLD))
    // {
    //     // shut off flapper
    //     pwm_start(FLAPPER_MOTOR, SERVO_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

    //     // stop wheels
    //     pwm_start(LEFT_WHEEL_A, SERVO_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    //     pwm_start(LEFT_WHEEL_B, SERVO_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    //     pwm_start(RIGHT_WHEEL_A, SERVO_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    //     pwm_start(RIGHT_WHEEL_B, SERVO_FREQ, 0, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    //     // delay(50);

    //     return true;
    // }
    return false;
}

bool grab_can()
{
    // close claw
    // pwm_start(CLAW_SERVO, SERVO_FREQ, CLAW_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    // delay(200);

    // // check reflectance -> or change this to a switch reading
    // if (analogRead(CLAW_SENSOR) < CAN_SENSING_THRESHOLD)
    // {
    //     return true;
    // }
    return false;
}

bool store_can()
{
    // if (analogRead(CLAW_SENSOR) > CAN_SENSING_THRESHOLD)
    // {
    //     reset_claw();
    //     return false;
    // }
    // delay(1000);

    // for (int i = ARM_DOWN; i < ARM_UP; i += 25)
    // {
    //     pwm_start(ARM_SERVO, SERVO_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    //     delay(15);
    // }

    // if (analogRead(CLAW_SENSOR) > CAN_SENSING_THRESHOLD)
    // {
    //     reset_claw();
    //     return false;
    // }
    // delay(1000);

    // for (int i = SWIVEL_ORIGIN; i > RESERVOIR_POSITIONS[reservoir_state]; i -= 50)
    // {
    //     pwm_start(SWIVEL_SERVO, SERVO_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    //     delay(30);
    // }

    // if (analogRead(CLAW_SENSOR) > CAN_SENSING_THRESHOLD)
    // {
    //     reset_claw();
    //     return false;
    // }
    // delay(1000);

    // pwm_start(CLAW_SERVO, SERVO_FREQ, CLAW_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

    // delay(1000);

    // for (int i = RESERVOIR_POSITIONS[reservoir_state]; i < SWIVEL_ORIGIN; i += 50)
    // {
    //     pwm_start(SWIVEL_SERVO, SERVO_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    //     delay(30);
    // }

    // delay(1000);

    // pwm_start(CLAW_SERVO, SERVO_FREQ, CLAW_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

    // delay(1000);

    // for (int i = ARM_UP; i > ARM_DOWN; i -= 25)
    // {
    //     pwm_start(ARM_SERVO, SERVO_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    //     delay(15);
    // }

    // delay(1000);

    // pwm_start(CLAW_SERVO, SERVO_FREQ, CLAW_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

    // delay(1000);

    // if (reservoir_state < 2)
    // {
    //     reservoir_state++;
    // }
    // else
    // {
    //     reservoir_state = 0;
    // }

    return true;
}

void reset_claw()
{
    pwm_start(SWIVEL_SERVO, SERVO_FREQ, SWIVEL_ORIGIN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(500);
    // change this to partially closed
    pwm_start(CLAW_SERVO, SERVO_FREQ, CLAW_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(500);
    pwm_start(ARM_SERVO, SERVO_FREQ, ARM_DOWN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(500);
    pwm_start(CLAW_SERVO, SERVO_FREQ, CLAW_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(500);
}

bool align()
{
    // check that the robot is in line with the return vehicle, i.e. the back sensor has been triggered
    // check that the robot is close enough, i.e. front and back sensors are reading values within the desired range
    
}

void check_state()
{
    static enum { INITIALIZE,
                  SEARCH,
                  GRAB_CAN,
                  STORE_CAN,
                  ALIGN,
                  DROPOFF } state = INITIALIZE;
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextColor(SSD1306_WHITE);
    display.print("state: ");
    display.println(state);
    // display.println("reflectance: ");
    // display.println(analogRead(CLAW_SENSOR));

    switch (state)
    {
    case INITIALIZE:
        // start-up sequence / waiting for the robot to touch ground, use tape sensors for this
        // reset_claw();
        state = SEARCH;
        break;
    case SEARCH:
        // has initialized, stored a can, or completed drop-off -> follow tape, flapper on
        if (at_dropoff)
        {
            state = ALIGN;
        }
        else if (search())
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
        if(align())
        {
            state = DROPOFF;
        }
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
    display.display();
}

void handle_interrupt()
{
    at_dropoff = true;
}