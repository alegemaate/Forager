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

#include "core/GpuProgram.h"
#include "game/Player.h"
#include "game/Skybox.h"
#include "utils/utils.h"

// Context
SDL_GLContext glcontext;

// Create fonts
asw::Font ARIAL_BLACK;

// Create images
asw::Texture cursor;

ChunkMap* gameTiles;
Player* jimmy;
Skybox* theSky;

void aswInit() {
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

  // Get window surface
  asw::display::renderer = SDL_CreateRenderer(asw::display::window, nullptr);

  SDL_SetRenderLogicalPresentation(asw::display::renderer, 1280, 960,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  glcontext = SDL_GL_CreateContext(asw::display::window);
  if (glcontext == nullptr) {
    asw::util::abortOnError("SDL_GL_CreateContext");
  }

  if (SDL_GL_MakeCurrent(asw::display::window, glcontext) < 0) {
    asw::util::abortOnError("SDL_GL_MakeCurrent");
  }

  asw::display::setTitle("Forager");
  asw::display::setIcon("assets/images/Forager.ico");

  cursor = asw::assets::loadTexture("assets/images/cursor2.png");

  // Hints
  SDL_GL_SetSwapInterval(1);
  // // Mouse sensitivity
  // set_mouse_speed(3, 3);
}

void openGlInit() {
  glewExperimental = GL_TRUE;

  // Glew
  GLenum err = glewInit();
  // GLEW can generate a benign GL_INVALID_ENUM right after init; clear it:
  glGetError();

  if (err != GLEW_OK) {
    abortOnError("Glew init failed.");
  }

  std::cout << "GL Vendor  : " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "GL Version : " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLEW       : " << glewGetString(GLEW_VERSION) << std::endl;

  // Viewport

  glViewport(0, 0, 1280, 960);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void loadShaders() {
  defaultShader = new GpuProgram("assets/shaders/textured.vert",
                                 "assets/shaders/textured.frag");
  waterShader =
      new GpuProgram("assets/shaders/water.vert", "assets/shaders/water.frag");
  skyShader =
      new GpuProgram("assets/shaders/sky.vert", "assets/shaders/sky.frag");
}

void gameInit() {
  // Camera
  camera = new Camera(glm::vec3(0.0f, 20.0f, 60.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f), -22.5f, -45.0f);

  // Character
  jimmy = new Player();

  // Sets Font
  // ARIAL_BLACK =
  //     asw::assets::loadFont("assets/images/fonts/arial_black.pcx", 16);

  // Load sky
  theSky = new Skybox();
  theSky->loadSkybox(
      "assets/images/skybox/front.png", "assets/images/skybox/back.png",
      "assets/images/skybox/left.png", "assets/images/skybox/right.png",
      "assets/images/skybox/top.png", "assets/images/skybox/bottom.png");

  gameTiles = new ChunkMap();
  gameTiles->generateMap();
}

// Load all in game content
void setup() {
  aswInit();
  openGlInit();
  loadShaders();
  gameInit();
}

// Run the game loops
void game() {
  asw::core::update();

  gameTiles->update();
  jimmy->update();

  // Gen
  if (asw::input::wasKeyPressed(asw::input::Key::R)) {
    gameTiles->generateMap();
  }

  // Change time
  skyTime += 0.000005f;

  if (asw::input::wasKeyPressed(asw::input::Key::KP_PLUS)) {
    skyTime += 0.005f;
  } else if (asw::input::wasKeyPressed(asw::input::Key::KP_MINUS)) {
    skyTime -= 0.005f;
  }

  if (skyTime > 1.0f) {
    skyTime = 0.0f;
  } else if (skyTime < 0.0f) {
    skyTime = 1.0f;
  }

  lightDir.y = -100.0f * cosf(2.0F * M_PI * skyTime);
  lightDir.x = 100.0f * cosf(2.0F * M_PI * skyTime);
  lightDir.z = -100.0f * sinf(2.0F * M_PI * skyTime);

  lightColor.x = -1.0f * powf((2.0F * skyTime) - 1, 2) + 1.15f;
  lightColor.y = -0.6f * (cosf(2.0F * M_PI * skyTime) - 1) + 0.05f;
  lightColor.z = -0.6f * (cosf(2.0F * M_PI * skyTime) - 1) + 0.05f;
}

void draw() {
  // const auto screenSize = asw::display::getLogicalSize();

  /**********************
   * OPEN GL DRAWING *
   **********************/

  // Clear screen
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw skybox
  theSky->render();

  // Draw map
  gameTiles->draw();

  GpuProgram::deactivate();

  SDL_GL_SwapWindow(asw::display::window);

  /**********************
   * ALLEGRO GL DRAWING *
   **********************/
  // allegro_gl_set_allegro_mode();

  // Transparent buffer
  // asw::draw::rectFill(asw::Quad<float>(0.0F, 0.0F, screenSize.x,
  // screenSize.y),
  //                     asw::util::makeColor(255, 0, 255));

  // // Cursor
  // asw::draw::sprite(cursor, asw::Vec2<float>((screenSize.x - 32) / 2,
  //                                            (screenSize.y - 32) / 2));

  // Debug text
  // textprintf_ex(
  //     buffer, ARIAL_BLACK, 20, 20, makecol(0, 0, 0), makecol(255, 255, 255),
  //     "Camera X:%.1f Y:%.1f Z:%.1f RotX:%.1f RotY:%.1f ", camera->position.x,
  //     camera->position.y, camera->position.z, camera->pitch, camera->yaw);
  // textprintf_ex(buffer, ARIAL_BLACK, 20, 60, makecol(0, 0, 0),
  //               makecol(255, 255, 255),
  //               "Light Direction X:%1.2f Y:%1.2f Z:%1.2f Time:%1.3f ",
  //               lightDir.x, lightDir.y, lightDir.z, skyTime);

  // Draws buffer

  // allegro_gl_unset_allegro_mode();
  // allegro_gl_flip();
}

int main() {
  // Setup game
  setup();

  while (!asw::core::exit &&
         !asw::input::wasKeyPressed(asw::input::Key::ESCAPE)) {
    game();
    draw();
  }

  SDL_GL_DestroyContext(glcontext);
  SDL_DestroyWindow(asw::display::window);
  SDL_Quit();

  return 0;
}
