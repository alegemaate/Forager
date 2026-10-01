/// @file settings_menu.h
///
/// Settings screen, shared by the title screen and the pause menu
///
#pragma once

#include "../game/settings.h"
#include "./menu.h"

class SettingsMenu
{
  public:
    explicit SettingsMenu(Settings& settings);

    // Items hold this menu's address
    SettingsMenu(const SettingsMenu&)            = delete;
    SettingsMenu& operator=(const SettingsMenu&) = delete;

    void open();

    bool isOpen() const
    {
        return opened;
    }

    void update();

    void draw(GuiRenderer& gui) const;

  private:
    void close();

    Settings& settings;
    Menu      menu{"Settings"};
    bool      opened{false};
};
