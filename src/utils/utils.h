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

extern bool collisionOverlap(double x1, double width, double x2, double width2);

// 3D Collision
extern bool collision3d(double x1,
                        double width1,
                        double x2,
                        double width2,
                        double y1,
                        double height1,
                        double y2,
                        double height2,
                        double z1,
                        double depth1,
                        double z2,
                        double depth2);

// Checks if file exists
extern bool fileExists(const char* filename);

// Random number generator
extern int random(int newLowest, int newHighest);

// Convert int to string
extern std::string convertIntToString(int number);

extern float distanceTo2D(float x1, float y1, float x2, float y2);
extern float distanceTo3D(float x1,
                          float y1,
                          float z1,
                          float x2,
                          float y2,
                          float z2);

// Error reporting
extern void abortOnError(const std::string& message);

#endif  // TOOLS_H
