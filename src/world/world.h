#pragma once

#include <asw/asw.h>

#include "../core/Camera.h"
#include "../game/Player.h"
#include "../game/settings.h"
#include "../render/gpu_program.h"
#include "../render/skybox.h"
#include "./chunk/chunk_map.h"

// World
class World
{
  public:
    World() = default;

    World(const World&)            = delete;
    World& operator=(const World&) = delete;

    /// @brief Load shaders and textures. Call once the GL context exists.
    void init();

    /// @brief Start a new world
    void start(u32 seed, const Settings& settings);

    /// @brief Drop the world's chunks
    void stop();

    /// @brief Game logic, at a fixed time step
    void update(float dt);

    /// @brief Load and mesh chunks. Call once per frame.
    void stream();

    void draw();

    /// @brief The player is placed and the chunks around them are drawn
    bool isReady() const
    {
        return spawned && getLoadProgress() >= 1.0f;
    }

    /// @brief Share of the starting area that is loaded, 0 to 1
    float getLoadProgress() const;

    /// @brief The camera is under water
    bool isUnderwater() const;

    /// @brief Set the time of day, 0 to 1, noon at 0.5
    void setTime(float newTime)
    {
        time = newTime;
        updateLight();
    }

    // Getters
    Camera& getCamera()
    {
        return camera;
    }
    const ChunkMap& getChunks() const
    {
        return chunks;
    }
    Player& getPlayer()
    {
        return player;
    }

  private:
    void trySpawn();
    void updateLight();

    ChunkMap  chunks;
    Player    player{};
    Skybox    skybox{};
    Camera    camera;

    const Settings* settings{nullptr};

    bool spawned{false};

    // Time of day, 0 to 1, noon at 0.5
    float time{0.4f};

    // Seconds since the world started, for animation
    float clock{0.0f};

    glm::vec3 lightDir{0.0f, 0.0f, 0.0f};
    glm::vec3 lightColor{0.0f, 0.0f, 0.0f};
    glm::vec3 lightAmbient{0.0f, 0.0f, 0.0f};
    glm::vec3 fogColor{0.0f, 0.0f, 0.0f};
    glm::vec3 skyTint{1.0f, 1.0f, 1.0f};

    GpuProgram worldShader;
};
