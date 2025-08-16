#include "Player.h"

#include <asw/asw.h>

#include "../utils/utils.h"

// Move character and such
void Player::update() {
  auto screenSize = asw::display::getLogicalSize();

  // Update camera
  camera->processMouseMovement(-((screenSize.x / 2) - asw::input::mouse.x),
                               (screenSize.y / 2) - asw::input::mouse.y);

  // Forward
  if (asw::input::isKeyDown(asw::input::Key::W) ||
      asw::input::isKeyDown(asw::input::Key::UP)) {
    camera->processKeyboard(CameraMovement::FORWARD, 1.0f);
  }
  // Backward
  if (asw::input::isKeyDown(asw::input::Key::S) ||
      asw::input::isKeyDown(asw::input::Key::DOWN)) {
    camera->processKeyboard(CameraMovement::BACKWARD, 1.0f);
  }
  // Left
  if (asw::input::isKeyDown(asw::input::Key::A) ||
      asw::input::isKeyDown(asw::input::Key::LEFT)) {
    camera->processKeyboard(CameraMovement::LEFT, 1.0f);
  }
  // Right
  if (asw::input::isKeyDown(asw::input::Key::D) ||
      asw::input::isKeyDown(asw::input::Key::RIGHT)) {
    camera->processKeyboard(CameraMovement::RIGHT, 1.0f);
  }

  // Reset mouse pos
  SDL_WarpMouseInWindow(asw::display::window, screenSize.x / 2,
                        screenSize.y / 2);
}
