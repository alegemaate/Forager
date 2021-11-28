

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

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
  // camera Attributes
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  // euler Angles
  float yaw;
  float pitch;
  float zoom;

  // camera options
  float movementSpeed = SPEED;
  float mouseSensitivity = SENSITIVITY;

  // constructor with vectors
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

  // returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
  }

  // processes input received from any keyboard-like input system. Accepts input
  // parameter in the form of camera defined ENUM (to abstract it from windowing
  // systems)
  void processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD) {
      position += front * velocity;
    }
    if (direction == BACKWARD) {
      position -= front * velocity;
    }
    if (direction == LEFT) {
      position -= right * velocity;
    }
    if (direction == RIGHT) {
      position += right * velocity;
    }
  }

  // processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void processMouseMovement(float xOffset,
                            float yOffset,
                            bool constrainPitch = true) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
      if (pitch > 89.0f) {
        pitch = 89.0f;
      }
      if (pitch < -89.0f) {
        pitch = -89.0f;
      }
    }

    // update front, right and Up Vectors using the updated Euler angles
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
  // calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // also re-calculate the right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
  }
};

#endif
