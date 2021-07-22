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
volatile int delta = 0;
volatile int m = 1; // Elapsed time in previous state
volatile int n = 0; // Elapsed time in current state
volatile int init_time = 0;
volatile int num = 0;
volatile int last_hiccup = 0; // Elapsed time from previous hiccup

volatile int robot_speed;

enum Robot_Position
{
    LEFT,
    RIGHT
};
Robot_Position prev_pos = LEFT;

// display relevant PID values on LCD screen
void display_values(int left_input, int right_input)
{
    display.print("L: ");
    display.print(left_input);
    display.print(" R: ");
    display.println(right_input);
    display.print("Speed: ");
    display.println(robot_speed);
    display.print("P: ");
    display.print(analogRead(P_POT));
    display.print(" I: ");
    display.print(analogRead(D_POT));
    display.print(" D: ");
    display.println(analogRead(P_POT));
    display.print("m: ");
    display.print(m);
    display.print(" n: ");
    display.print(n);
    display.print(" delta: ");
    display.println(delta);
    display.print("G: ");
    display.println(g);
    display.print("Err: ");
    display.print(error);
    display.print("  Pos: ");
    display.println(prev_pos);
}

// Param g less or equal to Cruising speed
void turn_wheels(int g, int speed)
{
    if (error == 0)
    {
        pwm_start(LEFT_WHEEL_A, DC_FREQ, speed, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(LEFT_WHEEL_B, DC_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);

        pwm_start(RIGHT_WHEEL_A, DC_FREQ, speed * RW_ADJUSTMENT_FACTOR, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(RIGHT_WHEEL_B, DC_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
    }
    else if (error < 0)
    {
        pwm_start(LEFT_WHEEL_A, DC_FREQ, speed + g, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(LEFT_WHEEL_B, DC_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);

        pwm_start(RIGHT_WHEEL_A, DC_FREQ, (speed - g) * RW_ADJUSTMENT_FACTOR, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(RIGHT_WHEEL_B, DC_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
    }
    else if (error > 0)
    {
        pwm_start(LEFT_WHEEL_A, DC_FREQ, speed - g, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(LEFT_WHEEL_B, DC_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);

        pwm_start(RIGHT_WHEEL_A, DC_FREQ, (speed + g) * RW_ADJUSTMENT_FACTOR, RESOLUTION_12B_COMPARE_FORMAT);
        pwm_start(RIGHT_WHEEL_B, DC_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
    }
}

void drive(int speed)
{
    int left_reading = analogRead(LEFT_TAPE_SENSOR);

    int right_reading = analogRead(RIGHT_TAPE_SENSOR);

    // int kp = analogRead(P_POT)*10;
    
    int kp = 140;
    int kd = analogRead(P_POT) * 10;
    int ki = analogRead(D_POT) * 10;
    robot_speed = analogRead(I_POT) * 5;

    // Finds error based on inputs from sensors
    if (left_reading > BW_THRES && right_reading > BW_THRES)
    {
        error = 0;
    }
    else if (left_reading < BW_THRES && right_reading > BW_THRES)
    {
        error = -2;
        prev_pos = LEFT;
    }
    else if (left_reading > BW_THRES && right_reading < BW_THRES)
    {
        error = 2;
        prev_pos = RIGHT;
    }
    else if (left_reading < BW_THRES && right_reading < BW_THRES)
    {
        if (prev_pos == LEFT)
        {
            error = -5;
        }
        else if (prev_pos == RIGHT)
        {
            error = 5;
        }
    }

    p = kp * error;
    d = kd * delta / (m + n);
    i = ki * error + i;

    if (i > MAX_INTEGRATOR_VALUE)
    {
        i = MAX_INTEGRATOR_VALUE;
    }
    else if (i < -MAX_INTEGRATOR_VALUE)
    {
        i = -MAX_INTEGRATOR_VALUE;
    }

    g = p + i + d;
    if (g < 0)
    {
        g = g * -1;
    }
    if (g > speed)
    {
        g = speed;
    }
    turn_wheels(g, robot_speed);
    
    if(error != lasterror){
        delta = error - lasterror;
        init_time = millis();
        m = n;
        n = 0;
    } else {
        n = (millis() - init_time) / 100;
    }
    lasterror = error;

    display_values(left_reading, right_reading);
}

bool search()
{
    if (at_dropoff)
    {
        pwm_start(FLAPPER_MOTOR, SERVO_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
        return true;
    }

    else 
    {
        if (millis() - last_hiccup > 3000)
        {
            pwm_start(FLAPPER_MOTOR, SERVO_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
            delay(20);
            pwm_start(ARM_SERVO, SERVO_FREQ, HICCUP, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
            delay(100);
            pwm_start(ARM_SERVO, SERVO_FREQ, ARM_DOWN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
            delay(100);
            last_hiccup = millis();
        }
        drive(CRUISING_SPEED);
    
        // start flapper
        pwm_start(FLAPPER_MOTOR, SERVO_FREQ, FLAPPER_SPEED, RESOLUTION_12B_COMPARE_FORMAT);

        if (analogRead(CLAW_SENSOR) < CAN_SENSING_THRESHOLD)
        {
            delay(50);
            pwm_start(CLAW_SERVO, SERVO_FREQ, CLAW_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

            // shut off flapper
            pwm_start(FLAPPER_MOTOR, SERVO_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
            
            // stop wheels
            pwm_start(LEFT_WHEEL_A, DC_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
            pwm_start(LEFT_WHEEL_B, DC_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
            pwm_start(RIGHT_WHEEL_A, DC_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);
            pwm_start(RIGHT_WHEEL_B, DC_FREQ, 0, RESOLUTION_12B_COMPARE_FORMAT);

        return true;
        }
    }
    return false;
}

bool store_can()
{
    delay(1000);
    for (int i = ARM_DOWN; i < ARM_UP; i += 25)
    {
        pwm_start(ARM_SERVO, SERVO_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(15);
    }

    for (int i = SWIVEL_ORIGIN; i > RESERVOIR_POSITIONS[reservoir_state]; i -= 50)
    {
        pwm_start(SWIVEL_SERVO, SERVO_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(15);
    }

    pwm_start(CLAW_SERVO, SERVO_FREQ, CLAW_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

    delay(500);

    for (int i = RESERVOIR_POSITIONS[reservoir_state]; i < SWIVEL_ORIGIN; i += 50)
    {
        pwm_start(SWIVEL_SERVO, SERVO_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(15);
    }
    delay(1000);

    pwm_start(CLAW_SERVO, SERVO_FREQ, CLAW_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

    delay(500);

    for (int i = ARM_UP; i > ARM_DOWN; i -= 25)
    {
        pwm_start(ARM_SERVO, SERVO_FREQ, i, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(15);
    }

    delay(500);

    pwm_start(CLAW_SERVO, SERVO_FREQ, CLAW_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(50);
    // only for blakes robot
    pwm_start(SWIVEL_SERVO, SERVO_FREQ, 2300, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

    delay(500);

    num++;
    if (num % 3 == 0)
    {
        pwm_start(RESERVOIR_SERVO, SERVO_FREQ, RESERVOIR_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(1000);
        pwm_start(RESERVOIR_SERVO, SERVO_FREQ, RESERVOIR_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(1000);
    }
    reservoir_state = num % 3;
    delay(3000);
    return true;
}

bool reset_claw()
{
    pwm_start(SWIVEL_SERVO, SERVO_FREQ, SWIVEL_ORIGIN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(1000);
    pwm_start(CLAW_SERVO, SERVO_FREQ, CLAW_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(800);
    pwm_start(ARM_SERVO, SERVO_FREQ, ARM_DOWN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(800);
    pwm_start(CLAW_SERVO, SERVO_FREQ, CLAW_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(400);
    pwm_start(RESERVOIR_SERVO, SERVO_FREQ, RESERVOIR_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(50);
    // Only need this for blakes robot
    // pwm_start(SWIVEL_SERVO, SERVO_FREQ, 2300, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    // delay(1000); 
    return true;
}

bool stop_drop_roll()
{
    // check that the robot is in line with the return vehicle, i.e. the front sensor has just left
    // the tape that is along the side of the return vehicle
    drive(DROPOFF_SPEED);
    if (digitalRead(FRONT_DROPOFF_SENSOR) == 0)
    {
        // stop wheels
        pwm_start(LEFT_WHEEL_A, DC_FREQ, 0, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
        pwm_start(LEFT_WHEEL_B, DC_FREQ, 0, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
        pwm_start(RIGHT_WHEEL_A, DC_FREQ, 0, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
        pwm_start(RIGHT_WHEEL_B, DC_FREQ, 0, TimerCompareFormat_t::RESOLUTION_16B_COMPARE_FORMAT);
        delay(50);
        pwm_start(RESERVOIR_SERVO, SERVO_FREQ, RESERVOIR_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        delay(1000);
        pwm_start(RESERVOIR_SERVO, SERVO_FREQ, RESERVOIR_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        return true;
    }
    return false;
}

void check_state()
{
    static enum { INITIALIZE,
                  SEARCH,
                  STORE_CAN,
                  STOP_DROP_ROLL } state = INITIALIZE;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.print("state: ");
    display.println(state);

    switch (state)
    {
    case INITIALIZE:
        // start-up sequence / waiting for the robot to touch ground, use tape sensors for this
        if (reset_claw())
        {
            state = SEARCH;
        }
        break;
    case SEARCH:
        // has initialized, stored a can, or completed drop-off -> follow tape, flapper on
        if (search())
        {
            if (at_dropoff)
            {
                state = STOP_DROP_ROLL;
            }
            else 
            {
                state = STORE_CAN;
            }
        }
        break;

    case STORE_CAN:
        // store can -> search
        if (store_can())
        {
            state = SEARCH;
        }
        break;

    case STOP_DROP_ROLL:
        // has aligned to the return vehicle
        // drive forwards slowly to drop cans, sense when the robot has reached the end of the return vehicle
        if (stop_drop_roll())
        {
            state = SEARCH;
        }
        break;

    default:
        state = INITIALIZE;
        break;
    }
    display.display();
}

void handle_interrupt()
{
    // at_dropoff = true;
}
