/*
  Forager!
  Allan Legemaate
  04/11/2015
  Foraging Game!
*/

// Includes
#include <GL/glew.h>

#include <alleggl.h>
#include <allegro.h>
#include <loadpng.h>

#include <string>

#include "core/GpuProgram.h"
#include "game/Player.h"
#include "game/Skybox.h"
#include "utils/utils.h"

// Create variables
int animationFrame = 0;
bool closeGame = false;

// Create fonts
FONT* ARIAL_BLACK;

// Create images
BITMAP* buffer;
BITMAP* cursor;

volatile int ticks = 0;
void ticker() {
  ticks++;
}
END_OF_FUNCTION(ticker)

// Animations
void animationTicker() {
  if (animationFrame == 0) {
    animationFrame = 1;
  } else if (animationFrame == 1) {
    animationFrame = 0;
  }
}
END_OF_FUNCTION(animationTicker)

TileMap* gameTiles;
Player* jimmy;
Skybox* theSky;

void allegroInit() {
  allegro_init();
  loadpng_init();
  install_allegro_gl();
  install_keyboard();
  install_timer();
  install_mouse();

  if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, ".")) {
    abortOnError(allegro_error);
  }

  set_color_depth(32);
  set_window_title("Forager");

  allegro_gl_set(AGL_Z_DEPTH, 32);
  allegro_gl_set(AGL_COLOR_DEPTH, 32);
  allegro_gl_set(AGL_SUGGEST, AGL_Z_DEPTH | AGL_COLOR_DEPTH);
  allegro_gl_set(AGL_REQUIRE, AGL_DOUBLEBUFFER);

  // Set screen mode
  int monitor_width = 0;
  int monitor_height = 0;
  get_desktop_resolution(&monitor_width, &monitor_height);

  if (set_gfx_mode(GFX_OPENGL_WINDOWED, monitor_width, monitor_height, 0, 0) !=
      0) {
    if (set_gfx_mode(GFX_OPENGL_WINDOWED, 1280, 960, 0, 0) != 0) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      abortOnError("Unable to go into any graphic mode\n%s\n");
    }
  }

  // Setup fNULLor FPS system
  LOCK_VARIABLE(ticks)
  LOCK_FUNCTION(ticker)
  install_int_ex(ticker, BPS_TO_TIMER(1000));

  LOCK_VARIABLE(animationFrame)
  LOCK_FUNCTION(animationTicker)
  install_int_ex(animationTicker, SECS_TO_TIMER(1));

  // Creates a buffer
  buffer = create_bitmap(SCREEN_W, SCREEN_H);

  cursor = load_bitmap("assets/images/cursor2.png", nullptr);

  // Mouse sensitivity
  set_mouse_speed(3, 3);
}

void openGlInit() {
  // Glew
  if (glewInit()) {
    abortOnError("Glew init failed.");
  }

  // Viewport
  glViewport(0, 0, SCREEN_W, SCREEN_H);
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
  ARIAL_BLACK =
      load_font("assets/images/fonts/arial_black.pcx", nullptr, nullptr);

  // Load sky
  theSky = new Skybox();
  theSky->loadSkybox(
      "assets/images/skybox/front.png", "assets/images/skybox/back.png",
      "assets/images/skybox/left.png", "assets/images/skybox/right.png",
      "assets/images/skybox/top.png", "assets/images/skybox/bottom.png");

  gameTiles = new TileMap();
  gameTiles->generateMap(buffer);
}

// Load all in game content
void setup() {
  allegroInit();
  openGlInit();
  loadShaders();
  gameInit();
}

// Run the game loops
void game() {
  gameTiles->update();
  jimmy->update();

  // Gen
  if (key[KEY_R]) {
    gameTiles->generateMap(buffer);
  }

  // Change time
  skyTime += 0.000005f;

  if (key[KEY_PLUS_PAD]) {
    skyTime += 0.005f;
  } else if (key[KEY_MINUS_PAD]) {
    skyTime -= 0.005f;
  }

  if (skyTime > 1.0f) {
    skyTime = 0.0f;
  } else if (skyTime < 0.0f) {
    skyTime = 1.0f;
  }

  lightDir.y = -100.0f * cosf(2.0f * M_PI * skyTime);
  lightDir.x = 100.0f * cosf(2.0f * M_PI * skyTime);
  lightDir.z = -100.0f * sinf(2.0f * M_PI * skyTime);

  lightColor.x = (-1.0f * powf((2.4f * skyTime) - 1.2f, 2) + 1.0f) + 0.1f;
  lightColor.y = -0.5f * (cosf(2.0f * M_PI * skyTime) - 1) + 0.1f;
  lightColor.z = -0.5f * (cosf(2.0f * M_PI * skyTime) - 1) + 0.1f;

  theSky->setTime(skyTime);

  // Exit game
  if (key[KEY_ESC]) {
    closeGame = true;
  }
}

void draw() {
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

  /**********************
   * ALLEGRO GL DRAWING *
   **********************/
  allegro_gl_set_allegro_mode();

  // Transparent buffer
  rectfill(buffer, 0, 0, SCREEN_W, SCREEN_H, makecol(255, 0, 255));

  // Cursor
  draw_sprite(buffer, cursor, (SCREEN_W - cursor->w) / 2,
              (SCREEN_H - cursor->h) / 2);

  // Debug text
  textprintf_ex(
      buffer, ARIAL_BLACK, 20, 20, makecol(0, 0, 0), makecol(255, 255, 255),
      "Camera X:%.1f Y:%.1f Z:%.1f RotX:%.1f RotY:%.1f ", camera->position.x,
      camera->position.y, camera->position.z, camera->pitch, camera->yaw);
  textprintf_ex(buffer, ARIAL_BLACK, 20, 60, makecol(0, 0, 0),
                makecol(255, 255, 255),
                "Light Direction X:%1.2f Y:%1.2f Z:%1.2f Time:%1.3f ",
                lightDir.x, lightDir.y, lightDir.z, skyTime);

  // Draws buffer
  draw_sprite(screen, buffer, 0, 0);

  allegro_gl_unset_allegro_mode();
  allegro_gl_flip();
}

int main() {
  // Setup game
  setup();

  // 120 Updates per second
  const constexpr double dt = 1000.0 / 120.0;

  double accumulator = 0.0;
  double current_time = ticks;
  double new_time;
  double frame_time;

  while (!closeGame && !key[KEY_ESC]) {
    new_time = ticks;
    frame_time = new_time - current_time;
    current_time = new_time;

    accumulator += frame_time;

    while (accumulator >= dt) {
      game();
      accumulator -= dt;
    }

    draw();
  }

  allegro_exit();

  return 0;
}
END_OF_MAIN()
