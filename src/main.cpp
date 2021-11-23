/*
  Forager!
  Allan Legemaate
  04/11/2015
  Foraging Game!
*/

// Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <alleggl.h>
#include <allegro.h>
#include <loadpng.h>

#include <cmath>
#include <ctime>
#include <iostream>
#include <string>

#include "core/Audio3d.h"
#include "game/MaterialManager.h"
#include "game/Player.h"
#include "utils/loaders.h"
#include "utils/shader.h"
#include "utils/utils.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

// Create variables
int animationFrame;
bool closeGame;

// Water
GLfloat waveTime, waveWidth, waveHeight, waveFreq;
GLint waveTimeLoc;
GLint waveWidthLoc;
GLint waveHeightLoc;

// Sky
GLint skyTimeLoc;

// Create fonts
FONT* ARIAL_BLACK;

// Create images
BITMAP* buffer;
BITMAP* cursor;

// Sounds
Audio3d* dinner;

Tile* sunTile;

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

// Load all in game content
void setup(bool first) {
  if (first) {
    /****************
     * SOME ALLEGRO *
     ****************/
    allegro_init();
    loadpng_init();
    install_allegro_gl();
    install_keyboard();
    install_timer();
    install_mouse();
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, ".")) {
      abort_on_error(allegro_error);
    }

    set_color_depth(32);
    set_window_title("Forager");

    /*******************
     * SOME ALLEGRO GL *
     *******************/
    allegro_gl_set(AGL_Z_DEPTH, 8);
    allegro_gl_set(AGL_COLOR_DEPTH, 32);
    allegro_gl_set(AGL_SUGGEST, AGL_Z_DEPTH | AGL_COLOR_DEPTH);
    allegro_gl_set(AGL_REQUIRE, AGL_DOUBLEBUFFER);

    // Set screen mode
    int monitor_width = 0;
    int monitor_height = 0;
    get_desktop_resolution(&monitor_width, &monitor_height);

    if (set_gfx_mode(GFX_OPENGL_WINDOWED, 1280, 960, 0, 0) != 0) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      abort_on_error("Unable to go into any graphic mode\n%s\n");
    }

    /****************
     * SOME OPEN GL *
     ****************/
    // I am setting a state where I am editing the projection matrix.
    glMatrixMode(GL_PROJECTION);

    // Clearing the projection matrix...
    glLoadIdentity();

    // set the perspective with the appropriate aspect ratio
    gluPerspective(55.0f, (GLfloat)SCREEN_W / (GLfloat)SCREEN_H, 0.1f, 200.0f);

    // Now editing the model-view matrix.
    glMatrixMode(GL_MODELVIEW);

    // Clearing the model-view matrix.
    glLoadIdentity();

    // Viewport
    glViewport(0, 0, SCREEN_W, SCREEN_H);

    // Alpha (remove pixels less than 0.5)
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_ALPHA_TEST);
    glShadeModel(GL_SMOOTH);

    // Set default material
    changeMaterial(MATERIAL_DEFAULT);

    glEnable(GL_DEPTH_TEST);

    // Cull back faces
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // TEXTURING
    // Enable texturing and blending (all tiles use this so let us just call it
    // once)
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);

    // Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /*************
     * SOME GLEW *
     *************/
    if (glewInit()) {
      abort_on_error("Glew init failed.");
    } else {
      std::cout << "Glew initialized \n\n";
    }

    // LOAD SHADERS
    std::cout << "   SHADERS\n-------------\n";

    // Shaders
    // DEFAULT
    GLuint vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    setupShader("data/shaders/textured.vert", vertexShader);
    setupShader("data/shaders/textured.frag", fragmentShader);

    // Make program
    defaultShader = glCreateProgram();

    glAttachShader(defaultShader, vertexShader);
    glAttachShader(defaultShader, fragmentShader);

    setupProgram(defaultShader);

    // WATER
    GLuint waterVertexShader, waterFragmentShader;
    waterVertexShader = glCreateShader(GL_VERTEX_SHADER);
    waterFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    setupShader("data/shaders/water.vert", waterVertexShader);
    setupShader("data/shaders/water.frag", waterFragmentShader);

    // Make program
    waterShader = glCreateProgram();

    glAttachShader(waterShader, waterVertexShader);
    glAttachShader(waterShader, waterFragmentShader);

    setupProgram(waterShader);

    // SKY
    GLuint skyVertexShader, skyFragmentShader;
    skyVertexShader = glCreateShader(GL_VERTEX_SHADER);
    skyFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    setupShader("data/shaders/sky.vert", skyVertexShader);
    setupShader("data/shaders/sky.frag", skyFragmentShader);

    // Make program
    skyShader = glCreateProgram();

    glAttachShader(skyShader, skyVertexShader);
    glAttachShader(skyShader, skyFragmentShader);

    setupProgram(skyShader);

    // Wave shader stuff
    waveTime = 1.5;
    waveWidth = 0.1;
    waveHeight = 0.8;
    waveFreq = 0.01;
    waveTimeLoc = glGetUniformLocation(waterShader, "waveTime");
    waveWidthLoc = glGetUniformLocation(waterShader, "waveWidth");
    waveHeightLoc = glGetUniformLocation(waterShader, "waveHeight");

    // Sky shader stuff
    skyTime = 0.4;
    skyTimeLoc = glGetUniformLocation(skyShader, "timer");

    // Sampler
    glUseProgram(skyShader);

    int my_sampler = glGetUniformLocation(skyShader, "sampler");
    glUniform1i(my_sampler, 1);

    my_sampler = glGetUniformLocation(skyShader, "tex");
    glUniform1i(my_sampler, 0);

    // Use our Shaders :D:D:D:D:D
    glUseProgram(defaultShader);

    // Enable lights
    glEnable(GL_LIGHTING);  // turns the "lights" on
    glEnable(GL_LIGHT0);

    // Lighting
    GLfloat light_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light_position[] = {1.5f, 2.0f, 1.0f, 0.0f};

    // Light 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // FPS STUFF
    // Creates a random number generator (based on time)
    srand(time(nullptr));

    // Setup fNULLor FPS system
    LOCK_VARIABLE(ticks)
    LOCK_FUNCTION(ticker)
    install_int_ex(ticker, BPS_TO_TIMER(1000));

    LOCK_VARIABLE(animationFrame)
    LOCK_FUNCTION(animationTicker)
    install_int_ex(animationTicker, SECS_TO_TIMER(1));

    // Creates a buffer
    buffer = create_bitmap(SCREEN_W, SCREEN_H);

    cursor = load_bitmap("images/cursor2.png", nullptr);

    // Mouse sensitivity
    set_mouse_speed(3, 3);

    // Character
    jimmy = new Player(0, 15, 0, 45, 135);

    // Sounds
    dinner = new Audio3d("sounds/dinner.wav", 0, 0, 0);

    // Sets Font
    ARIAL_BLACK = load_font("images/fonts/arial_black.pcx", nullptr, nullptr);

    // normal map
    // TRANSFORMS
    jimmy->transformWorld();

    GLuint samplerRef = loaders::loadTexture("images/skybox/sample.png");

    gameTiles = new TileMap(buffer);
    gameTiles->theSky.skyboxSampler = samplerRef;
    gameTiles->load_images();
    gameTiles->generateMap();

    sunX = -1.5;
    sunY = 1.5;
    sunTile =
        new Tile(sunX, sunY, sunZ, gameTiles->getManager()->getTileByType(1));

    // Load them models
    if (!quick_primitives::load_models()) {
      abort_on_error("quick_primitives couldn't load the damn model!");
    }
  }
}

// Run the game loops
void game() {
  gameTiles->update();
  jimmy->logic(gameTiles);

  if (key[KEY_M]) {
    dinner->play();
  }

  dinner->update(jimmy->getX(), jimmy->getY(), jimmy->getZ());

  /// Water Shader
  glUseProgram(waterShader);

  // Change time
  glUniform1f(waveTimeLoc, waveTime);
  glUniform1f(waveWidthLoc, waveWidth);
  glUniform1f(waveHeightLoc, waveHeight);

  // Update wave variable
  waveTime += waveFreq;

  /// Sky Shader
  glUseProgram(skyShader);

  // Change time
  glUniform1f(skyTimeLoc, skyTime);

  if (skyTime > 1)
    skyTime = 0;
  else if (skyTime < 0)
    skyTime = 1;

  skyTime += 0.000005;

  if (key[KEY_U]) {
    sunY += 0.05;
  } else if (key[KEY_J]) {
    sunY -= 0.05;
  }
  if (key[KEY_I]) {
    sunX += 0.05;
  } else if (key[KEY_K]) {
    sunX -= 0.05;
  }
  if (key[KEY_O]) {
    sunZ += 0.05;
  } else if (key[KEY_L]) {
    sunZ -= 0.05;
  }

  if (key[KEY_PLUS_PAD]) {
    skyTime += 0.005;
  } else if (key[KEY_MINUS_PAD]) {
    skyTime -= 0.005;
  }

  // Back to normal shader
  glUseProgram(defaultShader);

  // Orbit sun around world (i know.. i know.. that's not how it works)
  sunY = -1.0f * cosf(2.0f * M_PI * skyTime);
  sunX = 1.0f * cosf(2.0f * M_PI * skyTime);
  sunZ = -1.0f * sinf(2.0f * M_PI * skyTime);

  // Change light according to time (using cos! and parabolas!)
  // Red uses a parabola to simulate sunrise (more red so pink) and night
  // (less red so blue)
  // DAY
  float newRVal, newGVal, newBVal;
  newRVal = (-1.0f * powf((2.4f * skyTime) - 1.2f, 2) + 1.0f) + 0.1f;
  newGVal = -0.5f * (cosf(2.0f * M_PI * skyTime) - 1) + 0.1f;
  newBVal = -0.5f * (cosf(2.0f * M_PI * skyTime) - 1) + 0.1f;

  // Light color
  GLfloat light_ambient[] = {newRVal, newGVal, newBVal, 1.0f};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

  // Sunlight intensity
  GLfloat light_diffuse[] = {(0.6f + sunY) / 2, (0.6f + sunY) / 2,
                             (0.6f + sunY) / 2, 1.0f};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

  // Exit game
  if (key[KEY_ESC]) {
    closeGame = true;
  }
}

// Draw images
void draw() {
  // View matrix
  glMatrixMode(GL_MODELVIEW);

  // Clear screen
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset transforms
  glLoadIdentity();

  /*******************
   * OPEN GL DRAWING *
   *******************/

  // Default shader
  glUseProgram(defaultShader);

  // Transform world to players view
  jimmy->transformWorld();

  // Draw player
  jimmy->render();

  // Place light 0 Back to normal
  GLfloat light_position[] = {sunX, sunY, sunZ, 0.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  bool frameOn = int(skyTime * 60) % 2;

  sunTile->setX(static_cast<char>(sunX));
  sunTile->setY(static_cast<char>(sunY));
  sunTile->setZ(static_cast<char>(sunZ));
  sunTile->draw(frameOn);

  // Draw map
  if (!key[KEY_TILDE]) {
    gameTiles->draw(frameOn);
  }

  // Back to init shader
  glUseProgram(0);

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
  textprintf_ex(buffer, ARIAL_BLACK, 20, 20, makecol(0, 0, 0),
                makecol(255, 255, 255),
                "Camera X:%4.1f Y:%4.1f Z:%4.1f RotX:%4.1f RotY:%4.1f ",
                jimmy->getX(), jimmy->getY(), jimmy->getZ(),
                jimmy->getXRotation(), jimmy->getYRotation());
  textprintf_ex(
      buffer, ARIAL_BLACK, 20, 60, makecol(0, 0, 0), makecol(255, 255, 255),
      "Sun X:%1.2f Y:%1.2f Z:%1.2f Time:%1.3f ", sunX, sunY, sunZ, skyTime);

  // Draws buffer
  draw_sprite(screen, buffer, 0, 0);

  allegro_gl_unset_allegro_mode();

  allegro_gl_flip();
}

int main() {
  // Setup game
  setup(true);

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
      if (!key[KEY_1]) {
        game();
      }

      accumulator -= dt;
    }

    draw();
  }

  allegro_exit();

  return 0;
}
END_OF_MAIN()
