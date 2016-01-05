/*
  Tools
  Allan Legemaate
  11/11/15
  Holds all the quick access functions for the project
*/

#ifndef TOOLS_H
#define TOOLS_H

#include <allegro.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <math.h>

extern bool keyboard_keypressed();
extern bool joy_buttonpressed();

//Collision
extern bool collisionAny(int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionBottom(int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionTop(int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionRight(int xMin1, int xMax1, int xMin2, int xMax2);
extern bool collisionLeft(int xMin1, int xMax1, int xMin2, int xMax2);

// 3D Collision
extern bool collision3d(int x1, int width1, int x2, int width2, int y1, int height1, int y2, int height2, int z1, int depth1, int z2, int depth2);

// Mouse position including resolution difference
extern int mouseX();
extern int mouseY();

//Checks if file exists
extern bool fexists(const char *filename);

//Random number generator
extern int random(int newLowest, int newHighest);

//Convert int to string
extern std::string convertIntToString(int number);

//Convert double to string
extern std::string convertDoubleToString(double number);

//Convert bool to string
extern std::string convertBoolToString(bool boolean);

// Slope of line
extern double findSlope( int y_1, int y_2, int x_1, int x_2);

// Objects are close
extern bool closeTo( double val1, double val2, double maxDifference);

extern int distanceTo1D( int x1, int x2);
extern int distanceTo2D( int x1, int y1, int x2, int y2);
extern int distanceTo3D( int x1, int y1, int z1, int x2, int y2, int z2);

// Catch 0
extern int divideCatchZero( int number1, int number2);

// Frustum culling
//extern bool boxInFrustum( frustum3 const & fru, bound3 const & box );

// Fade in and out
extern void highcolor_fade_in(BITMAP* bmp_orig, int speed);
extern void highcolor_fade_out(int speed);

// Next state
extern void set_next_state( int newState );

// Error reporting
extern void abort_on_error(const char *message);


#endif // TOOLS_H
