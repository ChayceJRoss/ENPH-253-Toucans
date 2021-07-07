#include "fsm.h"

void check_state() 
{
    static enum {INITIALIZE, SEARCH, GRAB_CAN, STORE_CAN, ALIGN, DROPOFF} state = INITIALIZE;
    static uint32_t time_elapsed = 0;

    switch(state) 
    {
        case INITIALIZE:
        // start-up sequence / waiting for the robot to touch ground, use tape sensors for this
            break;
        case SEARCH:
        // has initialized, stored a can, or completed drop-off -> follow tape, flapper on
            break;

        case GRAB_CAN:
        // can has been sensed -> flapper off, stop driving, grab can
            break;

        case STORE_CAN:
        // can grabbed -> store can in reservoir
        // need memory for which reservoir slot to use? -> use global variable or something
        // TODO: Brian
            break;

        case ALIGN:
        // reached return vehicle -> stop flapper, change driving somehow, line up next to it
            break;

        case DROPOFF:
        // has aligned to the return vehicle
        // if no servo -> drive forwards slowly to drop cans, sense when the robot has reached the end of the return vehicle
        // if servo -> open the reservoir
            break;
        
        default:
            state = INITIALIZE;
            break;
    }
}

