#include "Player.h"

#include <asw/asw.h>

#include "../utils/utils.h"
#include "./World.h"

// Move character and such
void Player::update(World& world) {
  auto screenSize = asw::display::getSize();
  auto& camera = world.getCamera();

  // Update camera
  camera.processMouseMovement();

  // "Creative" flying mode
  if (flying) {
    // Forward
    if (asw::input::isKeyDown(asw::input::Key::W) ||
        asw::input::isKeyDown(asw::input::Key::UP)) {
      camera.processKeyboard(CameraMovement::FORWARD, 1.0f);
    }
    // Backward
    if (asw::input::isKeyDown(asw::input::Key::S) ||
        asw::input::isKeyDown(asw::input::Key::DOWN)) {
      camera.processKeyboard(CameraMovement::BACKWARD, 1.0f);
    }
    // Left
    if (asw::input::isKeyDown(asw::input::Key::A) ||
        asw::input::isKeyDown(asw::input::Key::LEFT)) {
      camera.processKeyboard(CameraMovement::LEFT, 1.0f);
    }
    // Right
    if (asw::input::isKeyDown(asw::input::Key::D) ||
        asw::input::isKeyDown(asw::input::Key::RIGHT)) {
      camera.processKeyboard(CameraMovement::RIGHT, 1.0f);
    }
  }

  // If not flying, apply gravity
  if (!flying) {
    camera.setPosition(camera.getPosition() - glm::vec3(0.0f, 0.01f, 0.0f));
  }

  // Toggle flying mode
  if (asw::input::wasKeyPressed(asw::input::Key::SPACE)) {
    flying = !flying;
  }

  // Reset mouse pos
  SDL_WarpMouseInWindow(asw::display::window, screenSize.x / 2,
                        screenSize.y / 2);
}
