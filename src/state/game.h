#pragma once

#include "../core/Types.h"
#include "../game/World.h"
#include "state.h"

using namespace core;

// Game screen of game
class Game : public asw::scene::Scene<ProgramState>
{
  public:
    using asw::scene::Scene<ProgramState>::Scene;

    void init() override;
    void update(f32 dt) override;
    void draw() override;

  private:
    World world;
    bool  fullscreen{false};
};
