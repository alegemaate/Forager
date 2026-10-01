/// @file menu.h
///
/// A list of buttons and settings, driven by keyboard, controller and mouse
///
#pragma once

#include <functional>
#include <string>
#include <vector>

#include "./gui_renderer.h"

struct MenuItem
{
    std::string label;

    /// @brief Value shown on the right, for settings. Empty for buttons.
    std::function<std::string()> value;

    /// @brief Called on accept or click
    std::function<void()> activate;

    /// @brief Called with -1 or 1 on left and right. Settings change their value here.
    std::function<void(int)> adjust;
};

class Menu
{
  public:
    explicit Menu(std::string title = "") : title(std::move(title)) {}

    void setItems(std::vector<MenuItem> newItems)
    {
        items = std::move(newItems);
        focus = 0;
    }

    void setTitle(std::string newTitle)
    {
        title = std::move(newTitle);
    }

    /// @brief Top of the menu, as a share of the screen height
    void setTop(f32 share)
    {
        top = share;
    }

    void resetFocus()
    {
        focus = 0;
    }

    /// @brief Handle input. Call once per update.
    void update();

    void draw(GuiRenderer& gui) const;

  private:
    struct Row
    {
        f32 x;
        f32 y;
        f32 w;
        f32 h;
    };

    Row layout(size_t index, const glm::vec2& screen) const;

    std::string           title;
    std::vector<MenuItem> items;
    size_t                focus{0};
    f32                   top{0.35f};
};
