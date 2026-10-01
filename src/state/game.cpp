#include "game.h"

#include <GL/glew.h>

#include "../game/controls.h"
#include "../gui/toolbar.h"

using namespace std;

void Game::init()
{
    world.init();

    gui.addElement(make_shared<Toolbar>());
}

void Game::update(f32 dt)
{
    if (asw::input::get_action_down(controls::QUIT))
    {
        asw::core::exit();
    }

    if (asw::input::get_action_down(controls::FULLSCREEN))
    {
        asw::display::set_fullscreen(!asw::display::is_fullscreen());
    }

    world.update(dt);
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

    auto&       camera    = world.getCamera();
    const auto& guiShader = world.getGpuProgramManager().getShader("gui");

    guiShader.activate();
    guiShader.setMat4("projection", camera.getOrthoMatrix());
    guiShader.setVec4("uColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // white tint

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    gui.render();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    asw::display::swap_window();
}
