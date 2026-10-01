#include "./settings_menu.h"

#include <algorithm>
#include <asw/asw.h>
#include <cmath>
#include <format>

#include "../game/controls.h"

namespace
{
std::string onOff(bool value)
{
    return value ? "On" : "Off";
}
} // namespace

SettingsMenu::SettingsMenu(Settings& settings) : settings(settings)
{
    menu.setTop(0.22f);

    auto& s = this->settings;

    const auto toggle = [this](bool& value)
    {
        return [this, &value](int)
        {
            value = !value;
            this->settings.apply();
        };
    };

    menu.setItems({
        {"Mouse speed", [&s] { return std::format("{:.1f}", s.mouseSensitivity); }, nullptr,
         [&s](int dir) { s.mouseSensitivity = std::clamp(s.mouseSensitivity + (0.1f * dir), 0.1f, 3.0f); }},
        {"Stick speed", [&s] { return std::format("{:.1f}", s.stickSensitivity); }, nullptr,
         [&s](int dir) { s.stickSensitivity = std::clamp(s.stickSensitivity + (0.1f * dir), 0.1f, 3.0f); }},
        {"Invert look", [&s] { return onOff(s.invertY); }, nullptr, [&s](int) { s.invertY = !s.invertY; }},
        {"Field of view", [&s] { return std::to_string(s.fieldOfView); }, nullptr, [&s](int dir)
         { s.fieldOfView = std::clamp(s.fieldOfView + (5 * dir), Settings::MIN_FOV, Settings::MAX_FOV); }},
        {"View distance", [&s] { return std::to_string(s.renderDistance); }, nullptr,
         [&s](int dir)
         {
             s.renderDistance =
                 std::clamp(s.renderDistance + dir, Settings::MIN_RENDER_DISTANCE, Settings::MAX_RENDER_DISTANCE);
         }},
        {"Volume", [&s] { return std::format("{}%", static_cast<int>(std::round(s.volume * 100.0f))); }, nullptr,
         [&s](int dir)
         {
             s.volume = std::clamp(s.volume + (0.1f * dir), 0.0f, 1.0f);
             s.apply();
         }},
        {"Fullscreen", [&s] { return onOff(s.fullscreen); }, nullptr, toggle(s.fullscreen)},
        {"Vsync", [&s] { return onOff(s.vsync); }, nullptr, toggle(s.vsync)},
        {"Back", nullptr, [this] { close(); }, nullptr},
    });
}

void SettingsMenu::open()
{
    opened = true;
    menu.resetFocus();
}

void SettingsMenu::close()
{
    opened = false;
    settings.save();
}

void SettingsMenu::update()
{
    if (asw::input::get_action_down(controls::UI_BACK))
    {
        close();
        return;
    }

    menu.update();
}

void SettingsMenu::draw(GuiRenderer& gui) const
{
    const glm::vec2 screen = gui.getSize();
    gui.rect(0.0f, 0.0f, screen.x, screen.y, glm::vec4(0.0f, 0.0f, 0.0f, 0.55f));
    menu.draw(gui);
}
