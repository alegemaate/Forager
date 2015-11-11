#ifndef GLOBALS_H
#define GLOBALS_H

//All libaries
#include <allegro.h>
#include <string>

using namespace std;

// Set next state
void set_next_state( int newState );

//Game states
enum GameStates
{
    STATE_NULL,
    STATE_INIT,
    STATE_INTRO,
    STATE_MENU,
    STATE_STORY,
    STATE_GAME,
    STATE_EXIT,
    STATE_MAP,
};

// Endgame Variables
extern bool distance_is_reached;
extern bool switch_flicked;

extern bool beaten_levels[10];

extern int level_distance;
extern float distance_travelled;
extern bool joystick_enabled;

// Frames
extern int frames_done;

//State variables
extern int stateID;
extern int nextState;
extern string levelOn;

#endif
