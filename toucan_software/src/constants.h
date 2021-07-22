#pragma once
// bluepill pin assignment
#define CLAW_SERVO PA_8
#define ARM_SERVO PA_9
#define SWIVEL_SERVO PA_10
#define RESERVOIR_SERVO PB_8

#define LEFT_TAPE_SENSOR PB0
#define RIGHT_TAPE_SENSOR PB1
#define FRONT_DROPOFF_SENSOR PB10
#define CLAW_SENSOR PA7
#define P_POT PA4
#define D_POT PA5
#define I_POT PA6

// A = forward, B = backward
// Brian wheels flipped, Blake and Tara regular
#define LEFT_WHEEL_A PA_0
#define LEFT_WHEEL_B PA_1
#define RIGHT_WHEEL_A PA_2
#define RIGHT_WHEEL_B PA_3
#define FLAPPER_MOTOR PB_9

// tara: 1.1
// brian: 0.9
// blake 1.0
const double RW_ADJUSTMENT_FACTOR = 0.9;
const int MAX_INTEGRATOR_VALUE = 100;

// Adafruit Display: PB6, PB7

const int SERVO_FREQ = 50;
const int DC_FREQ = 1000;
const int CRUISING_SPEED = 1000;
const int DROPOFF_SPEED = 1000;
const int FLAPPER_SPEED = 1000; 

// Servo Position
// MIN 500, MAX 2500
// blake - 690, brian - 720, tara = 750
const int CLAW_CLOSE = 750;
const int CLAW_OPEN = 950;
const int ARM_DOWN = 500;
// blake 1550
const int ARM_UP = 1550;
const int HICCUP = 800;
const int SWIVEL_ORIGIN = 2500;

// blake - bluecan {950. 1150. 1500}, default 1000 1300 1600
// brian - toucan too many {1150, 1450, 1800}
// tara - mr roboto {1200, 1500, 1900}
const int RESERVOIR_POSITIONS[3] = {1250, 1550, 1900};
const int RESERVOIR_OPEN = 1000;
const int RESERVOIR_CLOSE = 500;

const int CAN_SENSING_THRESHOLD = 300;
const int TIME_TO_GRAB_CAN_THRESHOLD = 500;
const int BW_THRES = 100; // Black > 100, White < 100 approx

// display set-up
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible
