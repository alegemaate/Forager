#pragma once

#include <asw/asw.h>
#include <string>

#include "../game/app_context.h"
#include "../gui/menu.h"
#include "../gui/settings_menu.h"
#include "state.h"

// Title screen: pick a seed and start a world
class Title : public asw::scene::Scene<ProgramState>
{
  public:
    Title(asw::scene::SceneManager<ProgramState>& manager, AppContext& context);

    void init() override;
    void update(float dt) override;
    void draw() override;
    void cleanup() override;

  private:
    void newSeed();
    void editSeed();
    void play();

    AppContext&  context;
    Menu         menu;
    SettingsMenu settingsMenu;
    std::string  seedText;
    float        clock{0.0f};
};
