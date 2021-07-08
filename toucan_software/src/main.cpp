#include "constants.h"
#include "fsm.h"

void set_up_display() {
    // setting up the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.display();
}

void init_pins()
{
  pinMode(CLAW_SERVO, OUTPUT);
  // pinMode(ARM_SERVO, OUTPUT);
  // pinMode(SWIVEL_SERVO, OUTPUT);
  pinMode(CLAW_SENSOR, INPUT);
  // pinMode(LEFT_TAPE_SENSOR, INPUT);
  // pinMode(RIGHT_TAPE_SENSOR, INPUT);
  // pinMode(LEFT_WHEEL_A, OUTPUT);
  // pinMode(LEFT_WHEEL_B, OUTPUT);
  // pinMode(RIGHT_WHEEL_A, OUTPUT);
  // pinMode(RIGHT_WHEEL_B, OUTPUT);
  // pinMode(FLAPPER_MOTOR, OUTPUT);
}

void init_claw()
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

void setup()
{
  // put your setup code here, to run once:
  // skycrane sequence?
  set_up_display();
  init_pins();
  // init_claw();
}

void loop()
{
  // put your main code here, to run repeatedly:
  display.clearDisplay();
  display.setCursor(0,0);

  check_state();

  display.display();
}
