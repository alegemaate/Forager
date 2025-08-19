#pragma once

#include <asw/asw.h>

#include "state.h"

// Init screen of game
class Init : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;

 private:
};
