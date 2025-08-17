

#pragma once

#include <asw/asw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement
enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
 public:
  Camera() = default;

  // Constructor with vectors
  explicit Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
      : position(position), worldUp(up), yaw(yaw), pitch(pitch) {
    updateCameraVectors();
  }

  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  const glm::mat4 getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
  }

  const glm::mat4 getProjectionMatrix() const {
    const auto screenSize = asw::display::getSize();
    return glm::perspective(glm::radians(zoom),
                            (float)screenSize.x / (float)screenSize.y,
                            Camera::NEAR_PLANE, Camera::FAR_PLANE);
  }

  // Processes input received from any keyboard-like input system
  void processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = Camera::MOVEMENT_SPEED * deltaTime;
    switch (direction) {
      case CameraMovement::FORWARD:
        position += front * velocity;
        break;
      case CameraMovement::BACKWARD:
        position -= front * velocity;
        break;
      case CameraMovement::LEFT:
        position -= right * velocity;
        break;
      case CameraMovement::RIGHT:
        position += right * velocity;
        break;
    }
  }

  // Processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void processMouseMovement(bool constrainPitch = true) {
    const auto xOffset = asw::input::mouse.xChange * Camera::MOUSE_SENSITIVITY;
    const auto yOffset = -asw::input::mouse.yChange * Camera::MOUSE_SENSITIVITY;

    yaw += xOffset;
    pitch += yOffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
      pitch = std::clamp(pitch, -89.0f, 89.0f);
    }

    const auto zOffset = asw::input::mouse.z * Camera::ZOOM_SENSITIVITY;
    zoom -= zOffset;
    zoom =
        std::clamp(zoom, 1.0f, 120.0f);  // Clamp zoom between 1.0f and 120.0f

    // Update front, right and Up Vectors using the updated Euler angles
    updateCameraVectors();
  }

 private:
  // Calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 newFront;
    newFront.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    newFront.y = sinf(glm::radians(pitch));
    newFront.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Re-calculate the right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
  }

  // Camera Attributes
  glm::vec3 position;
  glm::vec3 front{0.0f, 0.0f, -1.0f};
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  // Euler Angles
  float yaw{-90.0f};
  float pitch{0.0f};
  float zoom{90.0f};

  // Camera options
  static constexpr float MOVEMENT_SPEED = 0.5f;
  static constexpr float MOUSE_SENSITIVITY = 0.3f;
  static constexpr float ZOOM_SENSITIVITY = 5.0f;

  static constexpr float NEAR_PLANE = 0.1f;
  static constexpr float FAR_PLANE = 1000.0f;
};
