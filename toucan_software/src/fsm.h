#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "constants.h"

// PID control of the robot
void drive();

// Moves the robot by following the black tape and actively checks for cans
bool search();

// Grabs the can using the claw
bool grab_can();

// Claw and arm sequence to drop can into reservoir
bool store_can();

// Resets claw position
void reset_claw();

// checks the state of the FSM and executes the state
void check_state();
