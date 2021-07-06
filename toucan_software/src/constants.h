// bluepill pin assignment
#define CLAW_SERVO PB_7
#define ARM_SERVO PB_9
#define SWIVEL_SERVO PB_8

#define CLAW_SWITCH PB11
#define CLAW_SENSOR PA7

#define LEFT_TAPE_SENSOR PB0
#define RIGHT_TAPE_SENSOR PB1

// A = forward, B = backward
#define LEFT_WHEEL_A PA_0
#define LEFT_WHEEL_B PA_1
#define RIGHT_WHEEL_A PA_2
#define RIGHT_WHEEL_B PA_3

#define FRONT_DROPOFF_SENSOR PA4
#define BACK_DROPOFF_SENSOR PA5

#define FLAPPER_MOTOR PA_6

const int MOTOR_FREQ = 50;
const int DC_FREQ = 1000;

// servo positions
// MIN 500, MAX 2500
const int CLAW_CLOSE = 700;
const int CLAW_OPEN = 1050;
const int ARM_DOWN = 500;
const int ARM_UP = 1500;
const int SWIVEL_ORIGIN = 2500;
const int RESERVOIR_POSITIONS[3] = {1000, 1300, 1600};
