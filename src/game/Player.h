/*
  Player
  Allan Legemaate
  21/11/15
  The controllable player
*/

#pragma once

#include <glm/glm.hpp>

class World;

class Player {
 public:
  void update(World& world);

 private:
  bool flying = true;

  glm::vec3 velocity{0.0f, 0.0f, 0.0f};
};
