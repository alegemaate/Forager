#pragma once

#include "../game/World.h"
#include "state.h"

// Game screen of game
class Game : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;

 private:
  World world;
};
