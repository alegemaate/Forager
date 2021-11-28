#include "Player.h"

#include "../utils/utils.h"

// Move character and such
void Player::update() {
  // Update camera
  camera->processMouseMovement(-((SCREEN_W / 2) - mouse_x),
                               (SCREEN_H / 2) - mouse_y);

  // Forward
  if (key[KEY_W] || key[KEY_UP]) {
    camera->processKeyboard(Camera_Movement::FORWARD, 1.0f);
  }
  // Backward
  if (key[KEY_S] || key[KEY_DOWN]) {
    camera->processKeyboard(Camera_Movement::BACKWARD, 1.0f);
  }
  // Left
  if (key[KEY_A] || key[KEY_LEFT]) {
    camera->processKeyboard(Camera_Movement::LEFT, 1.0f);
  }
  // Right
  if (key[KEY_D] || key[KEY_RIGHT]) {
    camera->processKeyboard(Camera_Movement::RIGHT, 1.0f);
  }

  // Reset mouse pos
  position_mouse(SCREEN_W / 2, SCREEN_H / 2);
}
