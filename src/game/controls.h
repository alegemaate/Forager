/// @file controls.h
///
/// Named input actions, bound to keyboard, mouse and any connected controller
///
#pragma once

#include <string_view>

namespace controls
{

constexpr std::string_view MOVE_FORWARD = "move_forward";
constexpr std::string_view MOVE_BACK    = "move_back";
constexpr std::string_view MOVE_LEFT    = "move_left";
constexpr std::string_view MOVE_RIGHT   = "move_right";
constexpr std::string_view JUMP         = "jump";
constexpr std::string_view SNEAK        = "sneak";
constexpr std::string_view TOGGLE_FLY   = "toggle_fly";
constexpr std::string_view REGENERATE   = "regenerate";
constexpr std::string_view TIME_FORWARD = "time_forward";
constexpr std::string_view TIME_BACK    = "time_back";
constexpr std::string_view FULLSCREEN   = "fullscreen";
constexpr std::string_view QUIT         = "quit";

/// @brief Bind every action. Call once after asw is initialized.
void bind();

} // namespace controls
