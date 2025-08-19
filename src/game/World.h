#pragma once

#include "../core/Camera.h"
#include "../render/gpu_program_manager.h"
#include "./BiomeManager.h"
#include "./ChunkMap.h"
#include "./Player.h"
#include "./Skybox.h"
#include "./TileTypeManager.h"

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

  // Managers
  TileTypeManager& getTileManager() { return tileManager; }
  const GpuProgramManager& getGpuProgramManager() const {
    return gpuProgramManager;
  }
  const BiomeManager& getBiomeManager() const { return biomeManager; }

  // Lighting
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
  GpuProgramManager gpuProgramManager;
  TileTypeManager tileManager;
  BiomeManager biomeManager;
};
