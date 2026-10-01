#pragma once

#include "../block/block_registry.h"
#include "../core/Camera.h"
#include "../game/Player.h"
#include "../game/settings.h"
#include "../render/gpu_program_manager.h"
#include "../render/skybox.h"
#include "./biome/biome_registry.h"
#include "./chunk/chunk_map.h"

// World
class World
{
  public:
    World() = default;

    /// @brief Load shaders and textures. Call once the GL context exists.
    void init();

    /// @brief Start a new world
    void start(u32 seed, const Settings& settings);

    /// @brief Drop the world's chunks
    void stop();

    void update(float dt);
    void draw();

    // Getters
    Camera& getCamera()
    {
        return camera;
    }
    ChunkMap& getChunks()
    {
        return chunks;
    }
    Player& getPlayer()
    {
        return player;
    }

    // Managers
    const GpuProgramManager& getGpuProgramManager() const
    {
        return gpuProgramManager;
    }
    const BiomeRegistry& getBiomeRegistry() const
    {
        return BiomeRegistry;
    }

    // Lighting
    const glm::vec3& getLightDir() const
    {
        return lightDir;
    }
    const glm::vec3& getLightColor() const
    {
        return lightColor;
    }
    const glm::vec3& getLightAmbient() const
    {
        return lightAmbient;
    }

  private:
    ChunkMap chunks;
    Player   player{};
    Skybox   skybox{};

    float time{0.4f};
    u32   seed{0};

    const Settings* settings{nullptr};

    Camera    camera;
    glm::vec3 lightDir{0.0f, 0.0f, 0.0f};
    glm::vec3 lightColor{0.0f, 0.0f, 0.0f};
    glm::vec3 lightAmbient{0.0f, 0.0f, 0.0f};

    GpuProgramManager gpuProgramManager;
    BiomeRegistry     BiomeRegistry;
};
