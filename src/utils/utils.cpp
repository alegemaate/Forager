#include "utils.h"

#include <asw/asw.h>
#include <cmath>

bool collisionOverlap(double x1, double width1, double x2, double width2) {
  if (x1 - width1 < x2 + width2 && x1 - width1 > x2 - width2) {
    return true;
  }
  return false;
}

// 3D collision
bool collision3d(double x1,
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
                 double depth2) {
  // check the X axis
  if (std::abs(x1 - x2) < width1 + width2) {
    // check the Y axis
    if (std::abs(y1 - y2) < height1 + height2) {
      // check the Z axis
      if (std::abs(z1 - z2) < depth1 + depth2) {
        return true;
      }
    }
  }
  return false;
}

// Random number generator. Use int random(lowest,highest);
int random(int newLowest, int newHighest) {
  return asw::random::between(newLowest, newHighest);
}

// Returns distance 2D
float distanceTo2D(float x1, float y1, float x2, float y2) {
  return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Returns distance 2D
float distanceTo3D(float x1, float y1, float z1, float x2, float y2, float z2) {
  return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) +
               (z2 - z1) * (z2 - z1));
}

/*
 *  ERROR REPORTING
 */
void abortOnError(const std::string& message) {
  asw::util::abortOnError(message);
}
