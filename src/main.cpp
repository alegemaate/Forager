/*
  Forager!
  Allan Legemaate
  04/11/2015
  Foraging Game!
*/

// Includes
#include <GL/glew.h>

#include <asw/asw.h>
#include <string>

#include "./game/controls.h"
#include "./state/game.h"
#include "./state/init.h"
#include "./state/state.h"

void init()
{
    // SDL, sound, the window and an OpenGL 3.3 core context
    asw::core::init_opengl(1280, 960);

    asw::display::set_title("Forager");
    asw::display::set_icon("assets/images/Forager.ico");

    // On macOS 14+ SDL no longer pulls a launched app to the front, so the
    // window opens behind the terminal or editor that started it
    SDL_RaiseWindow(asw::display::get_window());

    // Vsync, unless an ASW_CONFIG file sets display.vsync
    if (!asw::config::has("display.vsync"))
    {
        SDL_GL_SetSwapInterval(1);
    }

    controls::bind();

    glewExperimental = GL_TRUE;

    // Glew
    GLenum err = glewInit();
    // GLEW can generate a benign GL_INVALID_ENUM right after init; clear it:
    glGetError();

    if (err != GLEW_OK)
    {
        asw::util::abort_on_error("Glew init failed.");
    }

    asw::log::info("Forager Initialized");
    asw::log::info("GL Vendor  : {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    asw::log::info("GL Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    asw::log::info("GL Version : {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    asw::log::info("GLEW       : {}", reinterpret_cast<const char*>(glewGetString(GLEW_VERSION)));

    asw::core::print_info();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

int main()
{
    init();

    auto app = asw::scene::SceneManager<ProgramState>();
    app.register_scene<Game>(ProgramState::Game, app);
    app.set_next_scene(ProgramState::Game);

    app.start();

    asw::core::shutdown();

    return 0;
}
