#include "controls.h"

#include <asw/asw.h>

namespace
{
using asw::input::ANY_CONTROLLER;
using asw::input::bind_action;
using asw::input::ControllerAxis;
using asw::input::ControllerAxisBinding;
using asw::input::ControllerButton;
using asw::input::ControllerButtonBinding;
using asw::input::Key;
using asw::input::KeyBinding;

// Key, arrow key, D-pad and left stick direction for one movement action
void bindMove(std::string_view name, Key key, Key arrow, ControllerButton dpad, ControllerAxis axis, bool positive)
{
    bind_action(name, KeyBinding{key});
    bind_action(name, KeyBinding{arrow});
    bind_action(name, ControllerButtonBinding{dpad, ANY_CONTROLLER});
    bind_action(name, ControllerAxisBinding{axis, ANY_CONTROLLER, 0.2f, positive});
}

void bindButton(std::string_view name, Key key, ControllerButton button)
{
    bind_action(name, KeyBinding{key});
    bind_action(name, ControllerButtonBinding{button, ANY_CONTROLLER});
}
} // namespace

void controls::bind()
{
    // Stick up is negative Y
    bindMove(MOVE_FORWARD, Key::W, Key::Up, ControllerButton::DPadUp, ControllerAxis::LeftY, false);
    bindMove(MOVE_BACK, Key::S, Key::Down, ControllerButton::DPadDown, ControllerAxis::LeftY, true);
    bindMove(MOVE_LEFT, Key::A, Key::Left, ControllerButton::DPadLeft, ControllerAxis::LeftX, false);
    bindMove(MOVE_RIGHT, Key::D, Key::Right, ControllerButton::DPadRight, ControllerAxis::LeftX, true);

    bindButton(JUMP, Key::Space, ControllerButton::A);
    bindButton(SNEAK, Key::LShift, ControllerButton::LeftStick);
    bindButton(TOGGLE_FLY, Key::Q, ControllerButton::Y);
    bindButton(REGENERATE, Key::R, ControllerButton::Back);
    bindButton(TIME_FORWARD, Key::KpPlus, ControllerButton::RightShoulder);
    bindButton(TIME_BACK, Key::KpMinus, ControllerButton::LeftShoulder);
    bindButton(FULLSCREEN, Key::F11, ControllerButton::Guide);
    bindButton(QUIT, Key::Escape, ControllerButton::Start);
    bind_action(SCREENSHOT, KeyBinding{Key::F2});
}
