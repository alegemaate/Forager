#pragma once

#include <memory>
#include <vector>

#include "./gui_element.h"

class Gui
{
  public:
    Gui() = default;

    void render()
    {
        for (const auto& element : elements)
        {
            element->render();
        }
    }

    void addElement(const std::shared_ptr<GuiElement>& element)
    {
        elements.push_back(element);
    }

  private:
    std::vector<std::shared_ptr<GuiElement>> elements;
};