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

void init() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
    asw::util::abortOnError("SDL_Init");
  }

  if (!TTF_Init()) {
    asw::util::abortOnError("TTF_Init");
  }

  // --- Set GL attributes before creating the window ---
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);  // Request OpenGL 3.x
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // Initialize SDL_mixer
  SDL_AudioSpec spec;
  spec.format = SDL_AUDIO_S16LE;
  spec.freq = 44100;
  spec.channels = 2;

  if (!Mix_OpenAudio(0, &spec)) {
    asw::util::abortOnError("Mix_OpenAudio");
  }

  asw::display::window =
      SDL_CreateWindow("", 1280, 960, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (asw::display::window == nullptr) {
    asw::util::abortOnError("WINDOW");
  }

  SDL_GLContext glcontext = SDL_GL_CreateContext(asw::display::window);
  if (glcontext == nullptr) {
    asw::util::abortOnError("SDL_GL_CreateContext");
  }

  if (!SDL_GL_MakeCurrent(asw::display::window, glcontext)) {
    asw::util::abortOnError("SDL_GL_MakeCurrent");
  }

  asw::display::setTitle("Forager");
  asw::display::setIcon("assets/images/Forager.ico");

  // Hints
  SDL_GL_SetSwapInterval(1);
  // // Mouse sensitivity
  // set_mouse_speed(3, 3);

  glewExperimental = GL_TRUE;

  // Glew
  GLenum err = glewInit();
  // GLEW can generate a benign GL_INVALID_ENUM right after init; clear it:
  glGetError();

  if (err != GLEW_OK) {
    asw::util::abortOnError("Glew init failed.");
  }

  std::cout << "GL Vendor  : " << glGetString(GL_VENDOR) << '\n';
  std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << '\n';
  std::cout << "GL Version : " << glGetString(GL_VERSION) << '\n';
  std::cout << "GLEW       : " << glewGetString(GLEW_VERSION) << '\n';

  // Viewport
  glViewport(0, 0, 1280, 960);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

int main() {
  init();

  auto app = asw::scene::SceneManager<ProgramState>();
  app.registerScene<Game>(ProgramState::Game, app);
  app.setNextScene(ProgramState::Game);

  app.start();

  return 0;
}
