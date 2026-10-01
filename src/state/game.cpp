#include "game.h"

#include <GL/glew.h>

#include "../game/controls.h"
#include "../gui/hud.h"
#include "../utils/screenshot.h"

Game::Game(asw::scene::SceneManager<ProgramState>& manager, AppContext& context)
    : Scene(manager), context(context), settingsMenu(context.settings)
{
    world.init();
    shutterSound = asw::assets::load_sample("assets/sounds/shutter.ogg");

    pauseMenu.setItems({
        {"Resume", nullptr, [this] { paused = false; }, nullptr},
        {"Settings", nullptr, [this] { settingsMenu.open(); }, nullptr},
        {"Quit to title", nullptr, [this] { this->manager.set_next_scene(ProgramState::Title); }, nullptr},
    });
}

void Game::init()
{
    paused = false;
    world.start(context.seed, context.settings);
}

void Game::cleanup()
{
    setMouseCaptured(false);
    world.stop();
}

void Game::updateMouse()
{
    // Look with the mouse while playing, point and click in menus
    const bool want    = !paused;
    const bool current = SDL_GetWindowRelativeMouseMode(asw::display::get_window());

    // Browsers only lock the pointer after a click, so ask again on each one
    const bool clicked = asw::input::get_mouse_button_down(asw::input::MouseButton::Left);

    if (want != current || (want && clicked))
    {
        setMouseCaptured(want);
    }
}

void Game::update(f32 dt)
{
    if (asw::input::get_action_down(controls::FULLSCREEN))
    {
        context.settings.fullscreen = !context.settings.fullscreen;
        context.settings.apply();
        context.settings.save();
    }

    if (asw::input::get_action_down(controls::SCREENSHOT))
    {
        takeScreenshot = true;
    }

    if (settingsMenu.isOpen())
    {
        settingsMenu.update();
    }
    else if (paused)
    {
        if (asw::input::get_action_down(controls::PAUSE) || asw::input::get_action_down(controls::UI_BACK))
        {
            paused = false;
        }
        else
        {
            pauseMenu.update();
        }
    }
    else
    {
        if (asw::input::get_action_down(controls::PAUSE))
        {
            paused = true;
            pauseMenu.resetFocus();
        }
        else
        {
            world.update(dt);
        }
    }

    updateMouse();
}

void Game::draw()
{
    // Follow the window, which changes size on resize, fullscreen and high density displays
    int width  = 0;
    int height = 0;
    SDL_GetWindowSizeInPixels(asw::display::get_window(), &width, &height);
    glViewport(0, 0, width, height);

    // Clear screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    world.draw();

    auto& gui = context.gui;
    gui.begin();
    hud::draw(gui);

    if (settingsMenu.isOpen())
    {
        settingsMenu.draw(gui);
    }
    else if (paused)
    {
        const glm::vec2 screen = gui.getSize();
        gui.rect(0.0f, 0.0f, screen.x, screen.y, glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));
        pauseMenu.draw(gui);
    }
    gui.end();

    if (takeScreenshot)
    {
        takeScreenshot          = false;
        const std::string saved = screenshot::saveTimestamped();
        asw::log::info("Screenshot {}", saved.empty() ? "failed" : saved);
        if (!saved.empty())
        {
            asw::sound::play(shutterSound, asw::sound::PlayOptions{.volume = 0.8f, .bus = asw::sound::Bus::Ui});
        }
    }

    asw::display::swap_window();
}
