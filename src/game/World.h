#pragma once

#include "../core/Camera.h"
#include "../core/ShaderManager.h"
#include "./ChunkMap.h"
#include "./Player.h"
#include "./Skybox.h"

// World
class World {
 public:
  World() = default;

  void init();
  void update(float dt);
  void draw();

  // Getters
  Camera& getCamera() { return camera; }
  ChunkMap& getChunks() { return chunks; }
  Player& getPlayer() { return player; }
  const ShaderManager& getShaderManager() const { return shaderManager; }

  const glm::vec3& getLightDir() const { return lightDir; }
  const glm::vec3& getLightColor() const { return lightColor; }

 private:
  ChunkMap chunks;
  Player player{};
  Skybox skybox{};

  float time{0.4f};

  Camera camera;
  glm::vec3 lightDir{0.0f, 0.0f, 0.0f};
  glm::vec3 lightColor{0.0f, 0.0f, 0.0f};
  ShaderManager shaderManager;
};
