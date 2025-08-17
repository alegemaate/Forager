#include "game.h"

#include <cmath>
#include <numbers>
#include <string>
#include <vector>

#include "../constants/globals.h"

void Game::init() {
  loadShaders();
  gameInit();
}

void Game::update(float dt) {
  if (asw::input::wasKeyPressed(asw::input::Key::ESCAPE)) {
    asw::core::exit = true;
  }

  gameTiles.update();
  jimmy.update();

  // Gen
  if (asw::input::wasKeyPressed(asw::input::Key::R)) {
    gameTiles.generateMap();
  }

  // Change time
  skyTime += 0.000005f;

  if (asw::input::isKeyDown(asw::input::Key::KP_PLUS)) {
    skyTime += 0.005f;
  } else if (asw::input::isKeyDown(asw::input::Key::KP_MINUS)) {
    skyTime -= 0.005f;
  }

  if (skyTime > 1.0f) {
    skyTime = 0.0f;
  } else if (skyTime < 0.0f) {
    skyTime = 1.0f;
  }

  lightDir.y = -100.0f * std::cosf(2.0F * std::numbers::pi * skyTime);
  lightDir.x = 100.0f * std::cosf(2.0F * std::numbers::pi * skyTime);
  lightDir.z = -100.0f * std::sinf(2.0F * std::numbers::pi * skyTime);

  lightColor.x = -1.0f * std::powf((2.0F * skyTime) - 1, 2) + 1.15f;
  lightColor.y =
      -0.6f * (std::cosf(2.0F * std::numbers::pi * skyTime) - 1) + 0.05f;
  lightColor.z =
      -0.6f * (std::cosf(2.0F * std::numbers::pi * skyTime) - 1) + 0.05f;
}

void Game::draw() {
  // Clear screen
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw skybox
  theSky.render();

  // Draw map
  gameTiles.draw();

  GpuProgram::deactivate();

  SDL_GL_SwapWindow(asw::display::window);
}

void Game::loadShaders() {
  defaultShader.initProgramFromFiles({"textured.vert", "textured.frag"});
}

void Game::gameInit() {
  // Camera
  camera = Camera(glm::vec3(0.0f, 20.0f, 60.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                  -22.5f, -45.0f);

  // Sets Font
  // font = asw::assets::loadFont("assets/images/fonts/arial_black.pcx", 16);

  // Load sky
  theSky.loadSkybox(
      "assets/images/skybox/front.png", "assets/images/skybox/back.png",
      "assets/images/skybox/left.png", "assets/images/skybox/right.png",
      "assets/images/skybox/top.png", "assets/images/skybox/bottom.png");

  gameTiles.generateMap();
}
