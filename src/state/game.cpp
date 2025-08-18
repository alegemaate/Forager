#include "game.h"

#include <cmath>
#include <numbers>
#include <string>
#include <vector>

void Game::init() {
  world.init();
}

void Game::update(float dt) {
  if (asw::input::wasKeyPressed(asw::input::Key::ESCAPE)) {
    asw::core::exit = true;
  }

  world.update(dt);
}

void Game::draw() {
  // Clear screen
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  world.draw();

  SDL_GL_SwapWindow(asw::display::window);
}
