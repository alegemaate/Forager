/// @file settings.h
///
/// Player settings, saved between runs. Desktop builds save to a file, web builds to the browser.
///
#pragma once

#include "../core/Types.h"

using namespace core;

struct Settings
{
    f32  mouseSensitivity{1.0f};
    f32  stickSensitivity{1.0f};
    bool invertY{false};
    i32  fieldOfView{75};
#ifdef __EMSCRIPTEN__
    i32 renderDistance{5};
#else
    i32 renderDistance{8};
#endif
    f32  volume{0.8f};
    bool fullscreen{false};
    bool vsync{true};

    static constexpr i32 MIN_RENDER_DISTANCE = 2;
    static constexpr i32 MAX_RENDER_DISTANCE = 16;
    static constexpr i32 MIN_FOV             = 50;
    static constexpr i32 MAX_FOV             = 110;

    /// @brief Read saved settings, keeping defaults for anything missing
    void load();

    void save() const;

    /// @brief Apply the settings that change the window and sound
    void apply() const;
};
