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

#include "./state/game.h"
#include "./state/init.h"
#include "./state/state.h"

void init()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD))
    {
        asw::util::abort_on_error("SDL_Init");
    }

    if (!TTF_Init())
    {
        asw::util::abort_on_error("TTF_Init");
    }

    // --- Set GL attributes before creating the window ---
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // Request OpenGL 3.x
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Initialize SDL_mixer
    // SDL_AudioSpec spec;
    // spec.format = SDL_AUDIO_S16;
    // spec.freq = 44100;
    // spec.channels = 2;

    // if (!Mix_OpenAudio(0, &spec)) {
    //   asw::util::abort_on_error("Mix_OpenAudio");
    // }

    asw::display::window = SDL_CreateWindow("", 1280, 960, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (asw::display::window == nullptr)
    {
        asw::util::abort_on_error("WINDOW");
    }

    SDL_GLContext glcontext = SDL_GL_CreateContext(asw::display::window);
    if (glcontext == nullptr)
    {
        asw::util::abort_on_error("SDL_GL_CreateContext");
    }

    if (!SDL_GL_MakeCurrent(asw::display::window, glcontext))
    {
        asw::util::abort_on_error("SDL_GL_MakeCurrent");
    }

    asw::display::set_title("Forager");
    asw::display::set_icon("assets/images/Forager.ico");

    // Hints
    SDL_GL_SetSwapInterval(1);
    // // Mouse sensitivity
    // set_mouse_speed(3, 3);

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

    // Viewport
    glViewport(0, 0, 1280, 960);
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

    return 0;
}
