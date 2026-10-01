#include "Player.h"

#include <asw/asw.h>

#include "../utils/utils.h"
#include "../world/world.h"
#include "controls.h"

// Move character and such
void Player::update(World& world)
{
    auto& camera = world.getCamera();
    auto& chunks = world.getChunks();

    // "Creative" flying mode
    if (flying)
    {
        // Forward
        if (asw::input::get_action(controls::MOVE_FORWARD))
        {
            velocity += camera.getFront() * 1.0f;
        }
        // Backward
        if (asw::input::get_action(controls::MOVE_BACK))
        {
            velocity += -camera.getFront() * 1.0f;
        }
        // Left
        if (asw::input::get_action(controls::MOVE_LEFT))
        {
            velocity += -camera.getRight() * 1.0f;
        }
        // Right
        if (asw::input::get_action(controls::MOVE_RIGHT))
        {
            velocity += camera.getRight() * 1.0f;
        }
    }

    // If not flying, apply gravity
    else
    {
        // Check if can move forward
        auto canMoveForward  = !chunks.getTile(camera.getPosition() + camera.getForward()).isSolid();
        auto canMoveBackward = !chunks.getTile(camera.getPosition() - camera.getForward()).isSolid();
        auto canMoveRight    = !chunks.getTile(camera.getPosition() + camera.getRight()).isSolid();
        auto canMoveLeft     = !chunks.getTile(camera.getPosition() - camera.getRight()).isSolid();

        auto isFalling = !chunks.getTile(camera.getPosition() + velocity + glm::vec3(0.0f, -1.0f, 0.0f)).isSolid();

        if (isFalling)
        {
            velocity.y -= 0.01f; // Apply gravity
        }
        else
        {
            velocity.y = 0.0f; // Reset vertical velocity if on ground
        }

        auto movementModifier = 0.1f;
        if (!isFalling && asw::input::get_action(controls::SNEAK))
        {
            movementModifier = 0.05f;
        }
        else if (isFalling)
        {
            movementModifier = 0.05f;
        }

        if (asw::input::get_action(controls::MOVE_FORWARD) && canMoveForward)
        {
            velocity += camera.getForward() * movementModifier;
        }

        if (asw::input::get_action(controls::MOVE_BACK) && canMoveBackward)
        {
            velocity += -camera.getForward() * movementModifier;
        }

        if (asw::input::get_action(controls::MOVE_LEFT) && canMoveLeft)
        {
            velocity += -camera.getRight() * movementModifier;
        }

        if (asw::input::get_action(controls::MOVE_RIGHT) && canMoveRight)
        {
            velocity += camera.getRight() * movementModifier;
        }

        if (asw::input::get_action(controls::JUMP) && !isFalling)
        {
            velocity.y = 0.2f; // Jump
        }
    }

    // Update camera position
    camera.setPosition(camera.getPosition() + velocity);

    if (flying)
    {
        velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    else
    {
        velocity.x = 0.0f;                        // Reset horizontal velocity
        velocity.z = 0.0f;                        // Reset horizontal velocity
        velocity.y = std::max(velocity.y, -0.2f); // Limit fall speed
    }

    // Toggle flying mode
    if (asw::input::get_action_down(controls::TOGGLE_FLY))
    {
        flying = !flying;
    }
}
