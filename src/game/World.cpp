#include "./World.h"

#include <asw/asw.h>

void World::init() {
  auto& defaultShader = gpuProgramManager.createShader("default");
  defaultShader.initProgramFromFiles({"textured.vert", "textured.frag"});

  // Camera
  camera = Camera(glm::vec3(0.0f, 20.0f, 60.0f), -22.5f, -45.0f);

  // Load biomes
  biomeManager.load("assets/data/biomes.json");

  // Load tiles
  tileManager.load("assets/data/tiles.json");

  // Load sky
  skybox.loadSkybox(
      "assets/images/skybox/front.png", "assets/images/skybox/back.png",
      "assets/images/skybox/left.png", "assets/images/skybox/right.png",
      "assets/images/skybox/top.png", "assets/images/skybox/bottom.png");

  // Generate map
  chunks.generate(*this);
}

void World::update(float dt) {
  chunks.update(*this);
  player.update(*this);

  // Gen
  if (asw::input::wasKeyPressed(asw::input::Key::R)) {
    chunks.generate(*this);
  }

  // Change time
  time += 0.00005f;

  if (asw::input::isKeyDown(asw::input::Key::KP_PLUS)) {
    time += 0.005f;
  } else if (asw::input::isKeyDown(asw::input::Key::KP_MINUS)) {
    time -= 0.005f;
  }

  if (time > 1.0f) {
    time = 0.0f;
  } else if (time < 0.0f) {
    time = 1.0f;
  }

  lightDir.y = -100.0f * std::cosf(2.0F * std::numbers::pi * time);
  lightDir.x = 100.0f * std::cosf(2.0F * std::numbers::pi * time);
  lightDir.z = -100.0f * std::sinf(2.0F * std::numbers::pi * time);

  lightColor.x = -1.0f * std::powf((2.0F * time) - 1, 2) + 1.15f;
  lightColor.y =
      -0.6f * (std::cosf(2.0F * std::numbers::pi * time) - 1) + 0.05f;
  lightColor.z =
      -0.6f * (std::cosf(2.0F * std::numbers::pi * time) - 1) + 0.05f;

  // Clamp between 0.1 and 1.0
  lightAmbient.x = -1.0f * std::powf((2.0F * time) - 1, 2) + 1.15f;
  lightAmbient.y = -1.0f * std::powf((2.0F * time) - 1, 2) + 1.15f;
  lightAmbient.z = -1.0f * std::powf((2.0F * time) - 1, 2) + 1.15f;
}

void World::draw() {
  // Draw skybox
  skybox.render(*this);

  // Draw map
  chunks.render(*this);
}
