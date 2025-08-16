#pragma once

#include <asw/asw.h>

#include "../core/GpuProgram.h"
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
  void loadShaders();
  void gameInit();

  // Create fonts
  asw::Font font;

  // Create images
  asw::Texture cursor;

  ChunkMap* gameTiles{nullptr};
  Player* jimmy{nullptr};
  Skybox* theSky{nullptr};
};
