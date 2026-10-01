#include "Player.h"

#include <algorithm>
#include <asw/asw.h>
#include <cmath>
#include <limits>

#include "../block/block_registry.h"
#include "../world/world.h"
#include "controls.h"

namespace
{
// Speeds in blocks per second
constexpr f32 WALK_SPEED  = 4.5f;
constexpr f32 SNEAK_SPEED = 1.8f;
constexpr f32 SWIM_SPEED  = 3.0f;
constexpr f32 FLY_SPEED   = 12.0f;

constexpr f32 GRAVITY         = 28.0f;
constexpr f32 MAX_FALL        = 50.0f;
constexpr f32 JUMP_SPEED      = 8.5f;
constexpr f32 WATER_GRAVITY   = 10.0f;
constexpr f32 MAX_SINK        = 2.5f;
constexpr f32 SWIM_UP_SPEED   = 3.0f;
constexpr f32 CLIMB_OUT_SPEED = 6.0f;

// Gap kept between the player and a block they stop against
constexpr f32 SKIN = 0.001f;

i32 toBlock(f32 value)
{
    return static_cast<i32>(std::floor(value + 0.5f));
}
} // namespace

void Player::spawn(const glm::vec3& feet)
{
    position = feet;
    velocity = glm::vec3(0.0f);
    onGround = false;
}

bool Player::overlapsBlock(const glm::ivec3& block) const
{
    const glm::vec3 min = position - glm::vec3(HALF_WIDTH, 0.0f, HALF_WIDTH);
    const glm::vec3 max = position + glm::vec3(HALF_WIDTH, HEIGHT, HALF_WIDTH);

    for (int axis = 0; axis < 3; axis++)
    {
        const auto b = static_cast<f32>(block[axis]);
        if (b - 0.5f >= max[axis] || b + 0.5f <= min[axis])
        {
            return false;
        }
    }
    return true;
}

bool Player::collides(const ChunkMap& chunks) const
{
    const glm::vec3 min = position - glm::vec3(HALF_WIDTH, 0.0f, HALF_WIDTH);
    const glm::vec3 max = position + glm::vec3(HALF_WIDTH, HEIGHT, HALF_WIDTH);

    for (i32 x = toBlock(min.x); x <= toBlock(max.x - SKIN); x++)
    {
        for (i32 y = toBlock(min.y); y <= toBlock(max.y - SKIN); y++)
        {
            for (i32 z = toBlock(min.z); z <= toBlock(max.z - SKIN); z++)
            {
                if (chunks.isSolidAt(x, y, z))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void Player::moveAxis(const ChunkMap& chunks, int axis, f32 amount)
{
    if (amount == 0.0f)
    {
        return;
    }

    position[axis] += amount;
    if (!collides(chunks))
    {
        return;
    }

    // Stop at the nearest face of the blocks now overlapped
    const glm::vec3 min = position - glm::vec3(HALF_WIDTH, 0.0f, HALF_WIDTH);
    const glm::vec3 max = position + glm::vec3(HALF_WIDTH, HEIGHT, HALF_WIDTH);

    f32 limit = amount > 0.0f ? std::numeric_limits<f32>::max() : std::numeric_limits<f32>::lowest();

    for (i32 x = toBlock(min.x); x <= toBlock(max.x - SKIN); x++)
    {
        for (i32 y = toBlock(min.y); y <= toBlock(max.y - SKIN); y++)
        {
            for (i32 z = toBlock(min.z); z <= toBlock(max.z - SKIN); z++)
            {
                if (!chunks.isSolidAt(x, y, z))
                {
                    continue;
                }

                const auto face = static_cast<f32>(glm::ivec3(x, y, z)[axis]);
                limit           = amount > 0.0f ? std::min(limit, face - 0.5f) : std::max(limit, face + 0.5f);
            }
        }
    }

    // Distance from the feet to each side of the player on this axis
    const f32 above = axis == 1 ? HEIGHT : HALF_WIDTH;
    const f32 below = axis == 1 ? 0.0f : HALF_WIDTH;

    if (amount > 0.0f)
    {
        position[axis] = limit - above - SKIN;
    }
    else
    {
        position[axis] = limit + below + SKIN;
        if (axis == 1)
        {
            onGround = true;
        }
    }

    velocity[axis] = 0.0f;
    if (axis != 1)
    {
        blocked = true;
    }
}

bool Player::liquidAt(const ChunkMap& chunks, f32 height) const
{
    const BlockID id = chunks.getBlock(toBlock(position.x), toBlock(position.y + height), toBlock(position.z));
    return BlockRegistry::get(id).isLiquid();
}

void Player::update(World& world, f32 dt)
{
    const auto& chunks = world.getChunks();
    auto&       camera = world.getCamera();

    // Wait for the ground to load
    if (!chunks.isLoadedAt(toBlock(position.x), toBlock(position.z)))
    {
        camera.setPosition(getEyePosition());
        return;
    }

    if (asw::input::get_action_down(controls::TOGGLE_FLY))
    {
        flying   = !flying;
        velocity = glm::vec3(0.0f);
    }

    // Walking direction from the camera. Sticks give part strength, so they walk slower when pushed less.
    const f32 ahead =
        asw::input::get_action_strength(controls::MOVE_FORWARD) - asw::input::get_action_strength(controls::MOVE_BACK);
    const f32 side =
        asw::input::get_action_strength(controls::MOVE_RIGHT) - asw::input::get_action_strength(controls::MOVE_LEFT);

    glm::vec3 wish = (camera.getForward() * ahead) + (camera.getRight() * side);
    wish.y         = 0.0f;
    if (glm::length(wish) > 1.0f)
    {
        wish = glm::normalize(wish);
    }

    const bool jump  = asw::input::get_action(controls::JUMP);
    const bool sneak = asw::input::get_action(controls::SNEAK);

    inWater = liquidAt(chunks, 0.2f) || liquidAt(chunks, 0.9f);

    if (flying)
    {
        velocity   = wish * FLY_SPEED;
        velocity.y = (static_cast<f32>(jump) - static_cast<f32>(sneak)) * FLY_SPEED * 0.7f;
    }
    else
    {
        f32 speed = WALK_SPEED;
        if (inWater)
        {
            speed = SWIM_SPEED;
        }
        else if (sneak)
        {
            speed = SNEAK_SPEED;
        }

        velocity.x = wish.x * speed;
        velocity.z = wish.z * speed;

        if (inWater)
        {
            velocity.y = std::max(velocity.y - (WATER_GRAVITY * dt), -MAX_SINK);
            if (jump)
            {
                // Pushing against a bank climbs out
                velocity.y = blocked ? CLIMB_OUT_SPEED : SWIM_UP_SPEED;
            }
        }
        else
        {
            velocity.y = std::max(velocity.y - (GRAVITY * dt), -MAX_FALL);
            if (onGround && jump)
            {
                velocity.y = JUMP_SPEED;
            }
        }
    }

    // One axis at a time, so the player slides along walls
    onGround = false;
    blocked  = false;
    moveAxis(chunks, 0, velocity.x * dt);
    moveAxis(chunks, 2, velocity.z * dt);
    moveAxis(chunks, 1, velocity.y * dt);

    camera.setPosition(getEyePosition());
}
