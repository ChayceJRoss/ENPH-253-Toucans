#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// display set-up
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible

#define PING PA9 // Trigger Pin of Ultrasonic Sensor
#define ECHO PA8 // Echo Pin of Ultrasonic Sensor
#define RETRACT_BUTTON PA0
#define PULLEY_RIGHT PB_0
#define PULLEY_LEFT PB_1
#define HOOK_SERVO PA_7

const double SLOW_THRESHOLD = 20.0;
const double RELEASE_THRESHOLD = 8.0;
const int SERVO_FREQ = 50;
const int HOOK_CLOSE = 500;
const int HOOK_OPEN = 2500;
const int LEFT_SLOW = 1560;
const int RIGHT_SLOW = 1440;
const int LEFT_FAST = 2200;
const int RIGHT_FAST = 800;
const int PULLEY_STOP = 1500;
volatile bool first_drop = true;

double distance_cm;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

double detect_distance();
void release_hook();
void retract_skycrane(bool duration);

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

void setup() {
  // put your setup code here, to run once:
  set_up_display();
  pinMode(PING, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(RETRACT_BUTTON, INPUT_PULLUP);
  pinMode(PULLEY_LEFT, OUTPUT);
  pinMode(PULLEY_RIGHT, OUTPUT);
  pinMode(HOOK_SERVO, OUTPUT);
  pwm_start(HOOK_SERVO, SERVO_FREQ, HOOK_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  pwm_start(PULLEY_LEFT, SERVO_FREQ, PULLEY_STOP, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  pwm_start(PULLEY_RIGHT, SERVO_FREQ, PULLEY_STOP, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  delay(500);
}

void loop() {
  if(digitalRead(RETRACT_BUTTON) == 0)
  {
    retract_skycrane(false);   
    pwm_start(HOOK_SERVO, SERVO_FREQ, HOOK_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT); 
  }
  else
  {
    pwm_start(PULLEY_LEFT, SERVO_FREQ, LEFT_FAST, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    pwm_start(PULLEY_RIGHT, SERVO_FREQ, RIGHT_FAST, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);

    distance_cm = detect_distance();
    if (distance_cm < SLOW_THRESHOLD)
    {
      // if (first_drop)
      // {
      //   first_drop = false;
      //   pwm_start(PULLEY_LEFT, SERVO_FREQ, PULLEY_STOP, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
      //   pwm_start(PULLEY_RIGHT, SERVO_FREQ, PULLEY_STOP, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
      //   delay(200);
      // }
      pwm_start(PULLEY_LEFT, SERVO_FREQ, LEFT_SLOW, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
      pwm_start(PULLEY_RIGHT, SERVO_FREQ, RIGHT_SLOW, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    }

    if (distance_cm < RELEASE_THRESHOLD)
    {
      pwm_start(PULLEY_LEFT, SERVO_FREQ, LEFT_FAST, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
      pwm_start(PULLEY_RIGHT, SERVO_FREQ, RIGHT_FAST, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
      delay(1500);
      release_hook();
      retract_skycrane(true);
      return;
    }
    delay(100);
  }
}

double detect_distance()
{
  display.clearDisplay();
  display.setCursor(0,0);
  long duration;
  double distance;
   digitalWrite(PING, LOW);
   delayMicroseconds(2);
   digitalWrite(PING, HIGH);
   delayMicroseconds(10);
   digitalWrite(PING, LOW);
   pinMode(ECHO, INPUT);
   duration = pulseIn(ECHO, HIGH);
   distance = duration * 0.034/2;
   display.println(distance);
   display.display();
   return distance;
}

void release_hook()
{
  pwm_start(HOOK_SERVO, SERVO_FREQ, HOOK_OPEN, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
}

void retract_skycrane(bool duration)
{
  pwm_start(PULLEY_LEFT, SERVO_FREQ, RIGHT_FAST, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  pwm_start(PULLEY_RIGHT, SERVO_FREQ, LEFT_FAST, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
  if (duration)
  {
    delay(2000);
    pwm_start(HOOK_SERVO, SERVO_FREQ, HOOK_CLOSE, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(7000);
    pwm_start(PULLEY_LEFT, SERVO_FREQ, PULLEY_STOP, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    pwm_start(PULLEY_RIGHT, SERVO_FREQ, PULLEY_STOP, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
    delay(60000);
  }
}