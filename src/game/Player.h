/*
  Player
  Allan Legemaate
  21/11/15
  The controllable player
*/

#pragma once

class World;

class Player {
 public:
  void update(World& world);

 private:
  bool flying = true;
};
