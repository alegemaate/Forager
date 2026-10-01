/*
  Player
  Allan Legemaate
  21/11/15
  The controllable player
*/

#pragma once

#include <glm/glm.hpp>

#include "../core/Types.h"

using namespace core;

class World;
class ChunkMap;

class Player
{
  public:
    static constexpr f32 HALF_WIDTH = 0.3f;
    static constexpr f32 HEIGHT     = 1.8f;
    static constexpr f32 EYE_HEIGHT = 1.62f;

    /// @brief Put the player's feet at a position and stop them
    void spawn(const glm::vec3& feet);

    void update(World& world, f32 dt);

    /// @brief Feet position, at the bottom middle of the player
    const glm::vec3& getPosition() const
    {
        return position;
    }

    glm::vec3 getEyePosition() const
    {
        return position + glm::vec3(0.0f, EYE_HEIGHT, 0.0f);
    }

    bool isFlying() const
    {
        return flying;
    }

    bool isOnGround() const
    {
        return onGround;
    }

    /// @brief Check if the player's body overlaps a block, so a block placed there would trap them
    bool overlapsBlock(const glm::ivec3& block) const;

  private:
    // Move along one axis and stop at solid blocks
    void moveAxis(const ChunkMap& chunks, int axis, f32 amount);
    bool collides(const ChunkMap& chunks) const;
    bool liquidAt(const ChunkMap& chunks, f32 height) const;

    glm::vec3 position{0.0f};
    glm::vec3 velocity{0.0f};

    bool flying{false};
    bool onGround{false};
    bool inWater{false};

    // A wall stopped the last horizontal move
    bool blocked{false};
};
