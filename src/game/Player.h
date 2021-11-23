/*
  Player
  Allan Legemaate
  21/11/15
  The controllable player
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "TileMap.h"

class Player {
 public:
  Player(float x, float y, float z, float xRot, float yRot);

  float getX() const { return x; }
  float getY() const { return y; }
  float getZ() const { return z; }

  float getXRotation() const { return xRotation; }
  float getYRotation() const { return yRotation; }

  void transformWorld() const;

  void render();
  void logic(TileMap* newMap);

 private:
  float x;
  float y;
  float z;

  float xRotation;
  float yRotation;

  float y_velocity = 0;

  bool sprinting = false;
};

#endif  // PLAYER_H
