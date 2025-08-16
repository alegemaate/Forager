#include "game.h"

#include <string>
#include <vector>

#include "../constants/globals.h"

void Game::init() {
  loadShaders();
  gameInit();

  // cursor = asw::assets::loadTexture("assets/images/cursor2.png");
}

void Game::update(float dt) {
  if (asw::input::wasKeyPressed(asw::input::Key::ESCAPE)) {
    asw::core::exit = true;
  }

  gameTiles->update();
  jimmy->update();

  // Gen
  if (asw::input::wasKeyPressed(asw::input::Key::R)) {
    gameTiles->generateMap();
  }

  // Change time
  skyTime += 0.000005f;

  if (asw::input::wasKeyPressed(asw::input::Key::KP_PLUS)) {
    skyTime += 0.005f;
  } else if (asw::input::wasKeyPressed(asw::input::Key::KP_MINUS)) {
    skyTime -= 0.005f;
  }

  if (skyTime > 1.0f) {
    skyTime = 0.0f;
  } else if (skyTime < 0.0f) {
    skyTime = 1.0f;
  }

  lightDir.y = -100.0f * cosf(2.0F * M_PI * skyTime);
  lightDir.x = 100.0f * cosf(2.0F * M_PI * skyTime);
  lightDir.z = -100.0f * sinf(2.0F * M_PI * skyTime);

  lightColor.x = -1.0f * powf((2.0F * skyTime) - 1, 2) + 1.15f;
  lightColor.y = -0.6f * (cosf(2.0F * M_PI * skyTime) - 1) + 0.05f;
  lightColor.z = -0.6f * (cosf(2.0F * M_PI * skyTime) - 1) + 0.05f;
}

void Game::draw() {
  // Clear screen
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw skybox
  theSky->render();

  // Draw map
  gameTiles->draw();

  GpuProgram::deactivate();

  SDL_GL_SwapWindow(asw::display::window);

  /**********************
   * ALLEGRO GL DRAWING *
   **********************/
  // allegro_gl_set_allegro_mode();

  // Transparent buffer
  // asw::draw::rectFill(asw::Quad<float>(0.0F, 0.0F, screenSize.x,
  // screenSize.y),
  //                     asw::util::makeColor(255, 0, 255));

  // // Cursor
  // asw::draw::sprite(cursor, asw::Vec2<float>((screenSize.x - 32) / 2,
  //                                            (screenSize.y - 32) / 2));

  // Debug text
  // textprintf_ex(
  //     buffer, ARIAL_BLACK, 20, 20, makecol(0, 0, 0), makecol(255, 255, 255),
  //     "Camera X:%.1f Y:%.1f Z:%.1f RotX:%.1f RotY:%.1f ", camera->position.x,
  //     camera->position.y, camera->position.z, camera->pitch, camera->yaw);
  // textprintf_ex(buffer, ARIAL_BLACK, 20, 60, makecol(0, 0, 0),
  //               makecol(255, 255, 255),
  //               "Light Direction X:%1.2f Y:%1.2f Z:%1.2f Time:%1.3f ",
  //               lightDir.x, lightDir.y, lightDir.z, skyTime);
}

void Game::loadShaders() {
  defaultShader = new GpuProgram("assets/shaders/textured.vert",
                                 "assets/shaders/textured.frag");
  waterShader =
      new GpuProgram("assets/shaders/water.vert", "assets/shaders/water.frag");
  skyShader =
      new GpuProgram("assets/shaders/sky.vert", "assets/shaders/sky.frag");
}

void Game::gameInit() {
  // Camera
  camera = new Camera(glm::vec3(0.0f, 20.0f, 60.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f), -22.5f, -45.0f);

  // Character
  jimmy = new Player();

  // Sets Font
  // font = asw::assets::loadFont("assets/images/fonts/arial_black.pcx", 16);

  // Load sky
  theSky = new Skybox();
  theSky->loadSkybox(
      "assets/images/skybox/front.png", "assets/images/skybox/back.png",
      "assets/images/skybox/left.png", "assets/images/skybox/right.png",
      "assets/images/skybox/top.png", "assets/images/skybox/bottom.png");

  gameTiles = new ChunkMap();
  gameTiles->generateMap();
}
