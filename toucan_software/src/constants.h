#pragma once
// bluepill pin assignment
#define CLAW_SERVO PA_8
#define ARM_SERVO PA_9
#define SWIVEL_SERVO PA_10
#define RESERVOIR_SERVO PB_8

#define LEFT_TAPE_SENSOR PB0
#define RIGHT_TAPE_SENSOR PB1
#define DROPOFF_SENSOR PA6
#define CLAW_SENSOR PA7
#define P_POT PA4
#define D_POT PA5

// A = forward, B = backward
// Brian wheels flipped, Blake and Tara regular
#define LEFT_WHEEL_A PA_3
#define LEFT_WHEEL_B PA_2
#define RIGHT_WHEEL_A PA_0
#define RIGHT_WHEEL_B PA_1
#define FLAPPER_MOTOR PB_9

// blake 1.0, tara 1.1, brian 0.9
const double RW_ADJUSTMENT_FACTOR = 1;
const double LW_ADJUSTMENT_FACTOR = 1.1;
const int MAX_INTEGRATOR_VALUE = 100;

// Adafruit Display: PB6, PB7

const int SERVO_FREQ = 50;
const int DC_FREQ = 1000;
const int CRUISING_SPEED = 1900;
const int G_THRESHOLD = 1000; // original threshold: 1600
// const int DROPOFF_SPEED = 1000;
const int FLAPPER_SPEED = 1000;
const int HICCUP = 600;

// Servo Position
// MIN 500, MAX 2500
// blake - 690, brian - 720, tara = 750
const int CLAW_CLOSE = 720;
const int CLAW_OPEN = 950;
const int ARM_DOWN = 500;
// blake 1550
const int ARM_UP = 1550;
const int SWIVEL_ORIGIN = 2500;
// blake {950, 1150, 1500}, tara {900, 1200, 1500}, brian {1150, 1450, 1800}, default 1000 1300 1600
const int RESERVOIR_POSITIONS[3] = {1150, 1450, 1800};
const int RESERVOIR_OPEN = 1000;
const int RESERVOIR_CLOSE = 500;

const int CAN_SENSING_THRESHOLD = 300;
const int DROPOFF_THRESHOLD = 100;

const int BW_THRES = 100; // Black > 100, White < 100 approx


// display set-up
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible 
