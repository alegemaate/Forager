#include "game.h"

#include <GL/gl.h>

#include "../gui/toolbar.h"

using namespace std;

void Game::init()
{
    world.init();

    gui.addElement(make_shared<Toolbar>());
}

void Game::update(f32 dt)
{
    if (asw::input::get_key_down(asw::input::Key::Escape))
    {
        asw::core::exit = true;
    }

    if (asw::input::get_key_down(asw::input::Key::F11))
    {
        fullscreen = !fullscreen;

        asw::display::set_fullscreen(fullscreen);
        SDL_SyncWindow(asw::display::window);

        auto screenSize = asw::display::get_size();

        if (fullscreen)
        {
            asw::display::set_resolution(screenSize.x, screenSize.y);
            glViewport(0, 0, screenSize.x, screenSize.y);
        }
        else
        {
            asw::display::set_resolution(1280, 960);
            glViewport(0, 0, 1280, 960);
        }
    }

    world.update(dt);
}

void Game::draw()
{
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

    SDL_GL_SwapWindow(asw::display::window);
}
