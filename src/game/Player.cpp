#include "Player.h"

#include <asw/asw.h>

#include "../utils/utils.h"
#include "../world/world.h"

// Move character and such
void Player::update(World& world)
{
    auto  ss     = asw::display::get_size();
    auto& camera = world.getCamera();
    auto& chunks = world.getChunks();

    // Update camera
    camera.processMouseMovement();

    // "Creative" flying mode
    if (flying)
    {
        // Forward
        if (asw::input::get_key(asw::input::Key::W) || asw::input::get_key(asw::input::Key::Up))
        {
            velocity += camera.getFront() * 1.0f;
        }
        // Backward
        if (asw::input::get_key(asw::input::Key::S) || asw::input::get_key(asw::input::Key::Down))
        {
            velocity += -camera.getFront() * 1.0f;
        }
        // Left
        if (asw::input::get_key(asw::input::Key::A) || asw::input::get_key(asw::input::Key::Left))
        {
            velocity += -camera.getRight() * 1.0f;
        }
        // Right
        if (asw::input::get_key(asw::input::Key::D) || asw::input::get_key(asw::input::Key::Right))
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
        if (!isFalling && asw::input::get_key(asw::input::Key::LShift))
        {
            movementModifier = 0.05f;
        }
        else if (isFalling)
        {
            movementModifier = 0.05f;
        }

        if ((asw::input::get_key(asw::input::Key::W) || asw::input::get_key(asw::input::Key::Up)) && canMoveForward)
        {
            velocity += camera.getForward() * movementModifier;
        }

        if ((asw::input::get_key(asw::input::Key::S) || asw::input::get_key(asw::input::Key::Down)) && canMoveBackward)
        {
            velocity += -camera.getForward() * movementModifier;
        }

        if ((asw::input::get_key(asw::input::Key::A) || asw::input::get_key(asw::input::Key::Left)) && canMoveLeft)
        {
            velocity += -camera.getRight() * movementModifier;
        }

        if ((asw::input::get_key(asw::input::Key::D) || asw::input::get_key(asw::input::Key::Right)) && canMoveRight)
        {
            velocity += camera.getRight() * movementModifier;
        }

        if (asw::input::get_key(asw::input::Key::Space) && !isFalling)
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
    if (asw::input::get_key_down(asw::input::Key::Q))
    {
        flying = !flying;
    }

    // Reset mouse pos
    SDL_WarpMouseInWindow(asw::display::window, ss.x / 2, ss.y / 2);
}
