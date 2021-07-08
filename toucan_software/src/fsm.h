#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "constants.h"

/** Drives the robot. Contains PID code
 */
void drive();

/** searches the floor for cans
 *  returns true if a can has been sensed in front of the claw, false otherwise
 */
bool search();

/** grabs a can
 *  returns true if the can was grabbed, false otherwise
 */
bool grab_can();

/** checks the state of the FSM and executes the state
 */
void check_state();

extern Adafruit_SSD1306 display;