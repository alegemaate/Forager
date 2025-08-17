#pragma once

#include <asw/asw.h>

#include "../core/GpuProgram.h"
#include "../game/ChunkMap.h"
#include "../game/Player.h"
#include "../game/Skybox.h"
#include "state.h"

// Game screen of game
class Game : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;

 private:
  ChunkMap gameTiles;
  Player jimmy{};
  Skybox theSky{};

  float skyTime{0.4f};
};
