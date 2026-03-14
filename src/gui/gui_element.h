#pragma once

#include <glm/glm.hpp>

#include "../core/Types.h"

using namespace core;

class GuiElement
{
  public:
    GuiElement()          = default;
    virtual ~GuiElement() = default;

    virtual void render() const {
        // Noop
    };

    /// @brief Set position
    void setPosition(glm::vec2 position)
    {
        this->position = position;
    }

  protected:
    glm::vec2 position{0, 0};
    glm::vec2 size{100, 30};
};