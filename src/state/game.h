#pragma once

#include <asw/asw.h>

#include "../core/Types.h"
#include "../game/app_context.h"
#include "../gui/menu.h"
#include "../gui/settings_menu.h"
#include "../world/world.h"
#include "state.h"

using namespace core;

// Game screen of game
class Game : public asw::scene::Scene<ProgramState>
{
  public:
    Game(asw::scene::SceneManager<ProgramState>& manager, AppContext& context);

    void init() override;
    void update(f32 dt) override;
    void draw() override;
    void cleanup() override;

  private:
    void drawLoading(GuiRenderer& gui) const;
    void updateMouse();

    AppContext&  context;
    World        world;
    Menu         pauseMenu{"Paused"};
    SettingsMenu settingsMenu;
    bool         paused{false};
    bool         takeScreenshot{false};
    asw::Sample  shutterSound;
};
