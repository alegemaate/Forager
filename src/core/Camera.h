

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement
enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
 public:
  // Camera Attributes
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  // Euler Angles
  float yaw;
  float pitch;
  float zoom;

  // Camera options
  float movementSpeed = SPEED;
  float mouseSensitivity = SENSITIVITY;

  // Constructor with vectors
  explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                  float yaw = YAW,
                  float pitch = PITCH)
      : position(position),
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        worldUp(up),
        yaw(yaw),
        pitch(pitch),
        zoom(ZOOM) {
    updateCameraVectors();
  }

  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
  }

  // Processes input received from any keyboard-like input system
  void processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == CameraMovement::FORWARD) {
      position += front * velocity;
    }
    if (direction == CameraMovement::BACKWARD) {
      position -= front * velocity;
    }
    if (direction == CameraMovement::LEFT) {
      position -= right * velocity;
    }
    if (direction == CameraMovement::RIGHT) {
      position += right * velocity;
    }
  }

  // Processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void processMouseMovement(float xOffset,
                            float yOffset,
                            bool constrainPitch = true) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
      if (pitch > 89.0f) {
        pitch = 89.0f;
      }
      if (pitch < -89.0f) {
        pitch = -89.0f;
      }
    }

    // Update front, right and Up Vectors using the updated Euler angles
    updateCameraVectors();
  }

  void processMouseScroll(float yOffset) {
    zoom -= (float)yOffset;
    if (zoom < 1.0f) {
      zoom = 1.0f;
    }
    if (zoom > 45.0f) {
      zoom = 45.0f;
    }
  }

 private:
  // Calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Re-calculate the right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
  }
};

#endif
