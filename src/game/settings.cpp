#include "./settings.h"

#include <algorithm>
#include <asw/asw.h>
#include <nlohmann/json.hpp>

namespace
{
constexpr auto SAVE_ORG  = "adsgames";
constexpr auto SAVE_APP  = "forager";
constexpr auto SAVE_NAME = "settings";
} // namespace

void Settings::load()
{
    const std::string text = asw::assets::read_save(SAVE_ORG, SAVE_APP, SAVE_NAME);
    if (text.empty())
    {
        return;
    }

    const auto json = nlohmann::json::parse(text, nullptr, false);
    if (json.is_discarded() || !json.is_object())
    {
        asw::log::warn("Ignoring settings that could not be read");
        return;
    }

    mouseSensitivity = std::clamp(json.value("mouseSensitivity", mouseSensitivity), 0.1f, 3.0f);
    stickSensitivity = std::clamp(json.value("stickSensitivity", stickSensitivity), 0.1f, 3.0f);
    invertY          = json.value("invertY", invertY);
    fieldOfView      = std::clamp(json.value("fieldOfView", fieldOfView), MIN_FOV, MAX_FOV);
    renderDistance = std::clamp(json.value("renderDistance", renderDistance), MIN_RENDER_DISTANCE, MAX_RENDER_DISTANCE);
    volume         = std::clamp(json.value("volume", volume), 0.0f, 1.0f);
    fullscreen     = json.value("fullscreen", fullscreen);
    vsync          = json.value("vsync", vsync);
}

void Settings::save() const
{
    const nlohmann::json json = {
        {"mouseSensitivity", mouseSensitivity},
        {"stickSensitivity", stickSensitivity},
        {"invertY", invertY},
        {"fieldOfView", fieldOfView},
        {"renderDistance", renderDistance},
        {"volume", volume},
        {"fullscreen", fullscreen},
        {"vsync", vsync},
    };

    if (!asw::assets::write_save(SAVE_ORG, SAVE_APP, SAVE_NAME, json.dump(2)))
    {
        asw::log::warn("Could not save settings");
    }
}

void Settings::apply() const
{
    asw::sound::set_master_volume(volume);

    // An ASW_CONFIG file can set these for every game, and then it wins
    if (!asw::config::has("display.fullscreen") && asw::display::is_fullscreen() != fullscreen)
    {
        asw::display::set_fullscreen(fullscreen);
    }

    if (!asw::config::has("display.vsync"))
    {
        SDL_GL_SetSwapInterval(vsync ? 1 : 0);
    }
}
