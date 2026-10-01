/// @file app_context.h
///
/// State shared by every scene
///
#pragma once

#include <asw/asw.h>

#include "../gui/gui_renderer.h"
#include "./settings.h"

struct AppContext
{
    Settings    settings;
    GuiRenderer gui;

    /// @brief Seed for the next world
    u32 seed{0};
};

/// @brief Hide the cursor and turn mouse movement into look input, or give the cursor back for menus
inline void setMouseCaptured(bool captured)
{
    SDL_SetWindowRelativeMouseMode(asw::display::get_window(), captured);
}
