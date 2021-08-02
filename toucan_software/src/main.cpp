#include "constants.h"
#include "fsm.h"

void set_up_display()
{
  // setting up the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.display();
}

void init_pins()
{
  pinMode(CLAW_SERVO, OUTPUT);
  pinMode(ARM_SERVO, OUTPUT);
  pinMode(SWIVEL_SERVO, OUTPUT);
  pinMode(CLAW_SENSOR, INPUT);
  pinMode(DROPOFF_SENSOR, INPUT);
  pinMode(LEFT_TAPE_SENSOR, INPUT);
  pinMode(RIGHT_TAPE_SENSOR, INPUT);
  pinMode(P_POT, INPUT);
  pinMode(D_POT, INPUT);
  // pinMode(I_POT, INPUT);
  pinMode(LEFT_WHEEL_A, OUTPUT);
  pinMode(LEFT_WHEEL_B, OUTPUT);
  pinMode(RIGHT_WHEEL_A, OUTPUT);
  pinMode(RIGHT_WHEEL_B, OUTPUT);
  pinMode(FLAPPER_MOTOR, OUTPUT);
}

void setup()
{
  // put your setup code here, to run once:
  // skycrane sequence?
  init_pins();
  set_up_display();
  reset_claw();
}

void loop()
{
  // put your main code here, to run repeatedly:
  check_state();
}
