#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "constants.h"

// Display relevant PID values on LCD screen
void display_values();

// adjusting the wheels for PID
void turn_wheels(int g, int speed);

// PID control of the robot
void drive();

// Moves the robot by following the black tape and actively checks for cans
bool search();

// Claw and arm sequence to drop can into reservoir
bool store_can();

// Resets claw position
bool reset_claw();

// Aligns the robot to the return vehicle
bool stop_drop_roll();

// checks the state of the FSM and executes the state
void check_state();

extern Adafruit_SSD1306 display;