#include "game.h"

using namespace std;

void Game::init()
{
    world.init();
}

void Game::update(f32 dt)
{
    if (asw::input::wasKeyPressed(asw::input::Key::ESCAPE))
    {
        asw::core::exit = true;
    }

    if (asw::input::wasKeyPressed(asw::input::Key::F11))
    {
        fullscreen = !fullscreen;

        asw::display::setFullscreen(fullscreen);
        SDL_SyncWindow(asw::display::window);

        auto screenSize = asw::display::getSize();

        if (fullscreen)
        {
            asw::display::setResolution(screenSize.x, screenSize.y);
            glViewport(0, 0, screenSize.x, screenSize.y);
        }
        else
        {
            asw::display::setResolution(1280, 960);
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

    SDL_GL_SwapWindow(asw::display::window);
}
