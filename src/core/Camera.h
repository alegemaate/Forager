#pragma once

#include <algorithm>
#include <asw/asw.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../game/settings.h"

// A first person camera that processes look input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
  public:
    Camera() = default;

    // Constructor with vectors
    Camera(glm::vec3 position, float yaw, float pitch) : position(position), yaw(yaw), pitch(pitch)
    {
        updateCameraVectors();
    }

    const glm::vec3& getPosition() const
    {
        return position;
    }

    void setPosition(const glm::vec3& pos)
    {
        position = pos;
    }

    void setRotation(float newYaw, float newPitch)
    {
        yaw   = newYaw;
        pitch = std::clamp(newPitch, -89.0f, 89.0f);
        updateCameraVectors();
    }

    void setFieldOfView(float degrees)
    {
        fieldOfView = degrees;
    }

    void setFarPlane(float distance)
    {
        farPlane = distance;
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix() const
    {
        auto up = glm::normalize(glm::cross(right, front));
        return glm::lookAt(position, position + front, up);
    }

    // Returns the projection matrix calculated using the field of view
    // and the aspect ratio of the display
    glm::mat4 getProjectionMatrix() const
    {
        const auto ss     = asw::display::get_size();
        const auto aspect = ss.y > 0 ? static_cast<float>(ss.x) / static_cast<float>(ss.y) : 1.0f;
        return glm::perspective(glm::radians(fieldOfView), aspect, Camera::NEAR_PLANE, farPlane);
    }

    // Looking direction
    const glm::vec3& getFront() const
    {
        return front;
    }

    // Looking direction, flat on the ground
    const glm::vec3& getForward() const
    {
        return forward;
    }

    const glm::vec3& getRight() const
    {
        return right;
    }

    // Turn the camera from mouse movement and the right stick. The mouse only counts while it is captured.
    void processLook(const Settings& settings, bool useMouse)
    {
        const auto&     mouse       = asw::input::get_mouse();
        const glm::vec2 mouseChange = useMouse ? glm::vec2(mouse.change.x, mouse.change.y) : glm::vec2(0.0f, 0.0f);
        const auto      stick =
            asw::input::get_controller_stick(asw::input::ANY_CONTROLLER, asw::input::ControllerStick::Right);

        const float mouseScale = Camera::MOUSE_SENSITIVITY * settings.mouseSensitivity;
        const float stickScale = Camera::STICK_SENSITIVITY * settings.stickSensitivity;
        const float invert     = settings.invertY ? -1.0f : 1.0f;

        yaw += (mouseChange.x * mouseScale) + (stick.x * stickScale);
        pitch -= ((mouseChange.y * mouseScale) + (stick.y * stickScale)) * invert;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        pitch = std::clamp(pitch, -89.0f, 89.0f);

        updateCameraVectors();
    }

  private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        front.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
        front.y = sinf(glm::radians(pitch));
        front.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
        front   = glm::normalize(front);

        // Re-calculate the right and Up vector
        right = glm::normalize(glm::cross(front, WORLD_UP));

        // Forward for walking, with no vertical part
        forward = glm::normalize(glm::vec3(cosf(glm::radians(yaw)), 0.0f, sinf(glm::radians(yaw))));
    }

    // Camera Attributes
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec3 front{0.0f, 0.0f, -1.0f};
    glm::vec3 right{1.0f, 0.0f, 0.0f};
    glm::vec3 forward{0.0f, 0.0f, -1.0f};

    // Euler Angles
    float yaw{-90.0f};
    float pitch{0.0f};

    float fieldOfView{75.0f};
    float farPlane{1000.0f};

    // Camera options, in degrees per point of mouse movement and per update at full stick
    static constexpr float MOUSE_SENSITIVITY = 0.15f;
    static constexpr float STICK_SENSITIVITY = 2.0f;

    static constexpr glm::vec3 WORLD_UP{0.0f, 1.0f, 0.0f};
    static constexpr float     NEAR_PLANE = 0.05f;
};
