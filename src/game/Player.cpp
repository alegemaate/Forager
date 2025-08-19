#include "Player.h"
#define GLM_ENABLE_EXPERIMENTAL

#include <asw/asw.h>
#include <glm/ext.hpp>

#include "../utils/utils.h"
#include "./World.h"

// Move character and such
void Player::update(World& world) {
  auto screenSize = asw::display::getSize();
  auto& camera = world.getCamera();
  auto& chunks = world.getChunks();

  // Update camera
  camera.processMouseMovement();

  // "Creative" flying mode
  if (flying) {
    // Forward
    if (asw::input::isKeyDown(asw::input::Key::W) ||
        asw::input::isKeyDown(asw::input::Key::UP)) {
      velocity += camera.getFront() * 1.0f;
    }
    // Backward
    if (asw::input::isKeyDown(asw::input::Key::S) ||
        asw::input::isKeyDown(asw::input::Key::DOWN)) {
      velocity += -camera.getFront() * 1.0f;
    }
    // Left
    if (asw::input::isKeyDown(asw::input::Key::A) ||
        asw::input::isKeyDown(asw::input::Key::LEFT)) {
      velocity += -camera.getRight() * 1.0f;
    }
    // Right
    if (asw::input::isKeyDown(asw::input::Key::D) ||
        asw::input::isKeyDown(asw::input::Key::RIGHT)) {
      velocity += camera.getRight() * 1.0f;
    }
  }

  // If not flying, apply gravity
  else {
    // Check if can move forward
    auto canMoveForward =
        !chunks.getTile(camera.getPosition() + camera.getForward()).isSolid();
    auto canMoveBackward =
        !chunks.getTile(camera.getPosition() - camera.getForward()).isSolid();
    auto canMoveRight =
        !chunks.getTile(camera.getPosition() + camera.getRight()).isSolid();
    auto canMoveLeft =
        !chunks.getTile(camera.getPosition() - camera.getRight()).isSolid();

    auto isFalling = !chunks
                          .getTile(camera.getPosition() + velocity +
                                   glm::vec3(0.0f, -1.0f, 0.0f))
                          .isSolid();

    if (isFalling) {
      velocity.y -= 0.01f;  // Apply gravity
    } else {
      velocity.y = 0.0f;  // Reset vertical velocity if on ground
    }

    auto movementModifier = 0.1f;
    if (!isFalling && asw::input::isKeyDown(asw::input::Key::LSHIFT)) {
      movementModifier = 0.05f;
    } else if (isFalling) {
      movementModifier = 0.05f;
    }

    if ((asw::input::isKeyDown(asw::input::Key::W) ||
         asw::input::isKeyDown(asw::input::Key::UP)) &&
        canMoveForward) {
      velocity += camera.getForward() * movementModifier;
    }

    if ((asw::input::isKeyDown(asw::input::Key::S) ||
         asw::input::isKeyDown(asw::input::Key::DOWN)) &&
        canMoveBackward) {
      velocity += -camera.getForward() * movementModifier;
    }

    if ((asw::input::isKeyDown(asw::input::Key::A) ||
         asw::input::isKeyDown(asw::input::Key::LEFT)) &&
        canMoveLeft) {
      velocity += -camera.getRight() * movementModifier;
    }

    if ((asw::input::isKeyDown(asw::input::Key::D) ||
         asw::input::isKeyDown(asw::input::Key::RIGHT)) &&
        canMoveRight) {
      velocity += camera.getRight() * movementModifier;
    }

    if (asw::input::isKeyDown(asw::input::Key::SPACE) && !isFalling) {
      velocity.y = 0.2f;  // Jump
    }
  }

  // Update camera position
  camera.setPosition(camera.getPosition() + velocity);

  if (flying) {
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
  } else {
    velocity.x = 0.0f;                         // Reset horizontal velocity
    velocity.z = 0.0f;                         // Reset horizontal velocity
    velocity.y = std::max(velocity.y, -0.2f);  // Limit fall speed
  }

  // Toggle flying mode
  if (asw::input::wasKeyPressed(asw::input::Key::Q)) {
    flying = !flying;
  }

  // Reset mouse pos
  SDL_WarpMouseInWindow(asw::display::window, screenSize.x / 2,
                        screenSize.y / 2);
}
