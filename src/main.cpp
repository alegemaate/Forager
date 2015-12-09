/*
  Forager!
  Allan Legemaate
  04/11/2015
  Foraging Game!
*/

//Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <allegro.h>
#include <alpng.h>
#include <alleggl.h>

#include <string>
#include <time.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <iostream>
#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

#include "ids.h"
#include "tile.h"
#include "tile_map.h"
#include "tools.h"
#include "player.h"

//Create variables
int gameScreen = INGAME;
int animationFrame;
bool closeGame;
bool showFPS;

//Create fonts
FONT *f1, *f2, *f3, *f4, *f5;

//Create images
BITMAP* buffer;
BITMAP* cursor;

//Resolution X
int resDiv;

//FPS System
int fps;
int frames_done;
int old_time;

// An array to store the number of frames we did during the last 10 tenths of a second
int frames_array[10];
int frame_index = 0;

volatile int ticks = 0;
void ticker(){
    ticks++;
}
END_OF_FUNCTION(ticker)

volatile int game_time = 0;
void game_time_ticker(){
    game_time++;
}
END_OF_FUNCTION(ticker)

const int updates_per_second = 60;

//Animations
void animationTicker(){
  if(animationFrame == 0){
    animationFrame = 1;
  }
  else if(animationFrame == 1){
    animationFrame = 0;
  }
}
END_OF_FUNCTION(ticker)

tile_map* gameTiles;
player* jimmy;

// Colours and properties of materials.
const GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat earth[] = { 0.6f, 0.6f, 0.5f, 1.0 };
const GLfloat sun[] = { 0.8f, 0.8f, 0.7f, 1.0 };
const GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat polished[] = { 10.0 };
const GLfloat dull[] = { 0.0 };

/******************
 * KILLMESHADERS! *
 ******************/
unsigned long getFileLength( std::ifstream &file){
    if(!file.good()) return 0;

    unsigned long pos=file.tellg();
    file.seekg(0,std::ios::end);
    unsigned long len = file.tellg();
    file.seekg(std::ios::beg);
    return len;
}

int loadshader(char* filename, GLchar** ShaderSource, unsigned long* len){
   std::ifstream file;
   file.open(filename, std::ios::in); // opens as ASCII!
   if(!file) return -1;

   *len = getFileLength(file);

   if (len==0) return -2;   // Error: Empty File

   *ShaderSource = (GLchar*)(new char[(int)len+1]);
   if (*ShaderSource == 0) return -3;   // can't reserve memory

    // len isn't always strlen cause some characters are stripped in ascii read...
    // it is important to 0-terminate the real length later, len is just max possible value...
   *ShaderSource[(int)len] = 0;

   unsigned int i=0;
   while (file.good())
   {
       *ShaderSource[i] = file.get();       // get character from file.
       if (!file.eof())
        i++;
   }

   *ShaderSource[i] = 0;  // 0-terminate it at the correct position

   file.close();

   return 0; // No Error
}

int unloadshader(GLubyte** ShaderSource){
   if (*ShaderSource != 0)
     delete[] *ShaderSource;
   *ShaderSource = 0;
}

GLuint vertexShader, fragmentShader;
/*********************
 * ENDKILLMESHADERS! *
 *********************/


//Load all ingame content
void setup(bool first){
  showFPS = true;

  if(first){
    /****************
     * SOME ALLEGRO *
     ****************/
    allegro_init();
    install_allegro_gl();
    alpng_init();
    install_keyboard();
    install_timer();
    install_mouse();
    set_color_depth( 32);
    set_window_title("Forager");

    /*******************
     * SOME ALLEGRO GL *
     *******************/
    allegro_gl_set(AGL_Z_DEPTH, 8);
    allegro_gl_set(AGL_COLOR_DEPTH, 32);
    allegro_gl_set(AGL_SUGGEST, AGL_Z_DEPTH | AGL_COLOR_DEPTH);
    allegro_gl_set(AGL_REQUIRE, AGL_DOUBLEBUFFER);

    //Set screenmode
    if(true == true){
      resDiv = 1;
      if(set_gfx_mode( GFX_OPENGL_WINDOWED, 1280, 960, 0, 0) !=0){
        resDiv = 2;
        if(set_gfx_mode( GFX_OPENGL_WINDOWED, 640, 480, 0, 0) !=0){
          resDiv = 4;
          if(set_gfx_mode( GFX_OPENGL_WINDOWED, 320, 240, 0, 0) !=0){
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
          	abort_on_error("Unable to go into fullscreen graphic mode\n%s\n");
          }
        }
      }
    }
    else{
      resDiv = 1;
      if(set_gfx_mode( GFX_OPENGL_WINDOWED, 1280, 960, 0, 0) !=0){
        resDiv = 2;
        if(set_gfx_mode( GFX_OPENGL_WINDOWED, 640, 480, 0, 0) !=0){
          resDiv = 4;
          if(set_gfx_mode( GFX_OPENGL_WINDOWED, 320, 240, 0, 0) !=0){
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
          	abort_on_error("Unable to set any windowed graphic mode\n%s\n");
          }
        }
      }
    }

    /****************
     * SOME OPEN GL *
     ****************/
    //I am setting a state where I am editing the projection matrix.
    glMatrixMode(GL_PROJECTION);

    //Clearing the projection matrix...
    glLoadIdentity();

    // set the perspective with the appropriate aspect ratio
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.2, 1000.0);

    //Now editing the model-view matrix.
    glMatrixMode(GL_MODELVIEW);

    //Clearing the model-view matrix.
    glLoadIdentity();

    // Viewport
    glViewport(0, 0, SCREEN_W, SCREEN_H);

    // Alpha (remove pixels less than 0.5)
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_ALPHA_TEST);
    glShadeModel (GL_SMOOTH);

    // Lighting
    /*GLfloat mat_specular[] = { 0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat mat_diffuse[]  ={ 1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_ambient[] = { 0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat mat_shininess[] = { 50.0f };*/

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, earth);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT, GL_SHININESS, polished);

    GLfloat light_ambient[] = { 0.4f, 0.4f, 0.3f, 1.0f };
    GLfloat light_ambient2[] = { 0.2f, 0.2f, 0.1f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_diffuse2[] = { 0.8f, 0.6f, 0.4f, 1.0f };
    GLfloat light_specular[] = { 0.9f, 0.9f, 1.0f, 1.0f };
    GLfloat light_position[] = { 0.0001, 0.0001, 1.0001, 0.0f };

    // Light 1
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Light 2
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);

    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

    // Enable lights
    glEnable(GL_LIGHTING); //turns the "lights" on
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glEnable( GL_DEPTH_TEST);

    /*************
     * SOME GLEW *
     *************/
    //glewExperimental = TRUE;
    if(glewInit())
      abort_on_error("Crap bukkits! Glew init failed.");

    // Shaders
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // TEXTURING
    // Enable texturing and blending (all tiles use this so lets just call it once)
    glEnable(GL_TEXTURE_2D);

    // Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // FPS STUFF
    //Creates a random number generator (based on time)
    srand (time(NULL));

    //Setup for FPS system
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

    LOCK_VARIABLE(game_time);
    LOCK_FUNCTION(game_time_ticker);
    install_int_ex(game_time_ticker, BPS_TO_TIMER(10));

    LOCK_VARIABLE(animationFrame);
    LOCK_FUNCTION(animationTicker);
    install_int_ex(animationTicker, SECS_TO_TIMER(1));

    fps = 0;
    frames_done = 0;
    old_time = 0;

    for(int ii = 0; ii < 10; ii++)
      frames_array[ii] = 0;

    //Creates a buffer
    buffer = create_bitmap( 1280, 960);

    cursor = load_bitmap( "images/cursor.png", NULL);

    //normal map
    gameTiles = new tile_map( buffer);
    gameTiles -> load_images();
    gameTiles -> generateMap();

    // Character
    jimmy = new player( 5, 5, 10);

    //Sets Font
    f1 = load_font("images/fonts/arial_black.pcx", NULL, NULL);
    f2 = extract_font_range(f1, ' ', 'A'-1);
    f3 = extract_font_range(f1, 'A', 'Z');
    f4 = extract_font_range(f1, 'Z'+1, 'z');

    //Merge fonts
    //font = merge_fonts(f4, f5 = merge_fonts(f2, f3));
  }
}

//Run the game loops
void game(){
  if(gameScreen == SPLASH){

  }
  else if(gameScreen == MENU){

  }
  else if(gameScreen == LEVELSELECT){

  }
  else if(gameScreen == INGAME){
    if( !key[KEY_TILDE])
      gameTiles -> update();
  }

  //Exit game
  if(key[KEY_ESC]){
    closeGame = true;
  }

  //Counter for FPS
  frames_done++;
}

//Draw images
void draw(){
  // View matrix
  glMatrixMode(GL_MODELVIEW);

  // Clear screen
  glClearColor(0.1f, 0.5f, 0.9f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset transforms
  glLoadIdentity();

  /*******************
   * OPEN GL DRAWING *
   *******************/
  // Draw map
  if( !key[KEY_TILDE])
    gameTiles -> draw( animationFrame);


  /**********************
   * ALLEGRO GL DRAWING *
   **********************/
  allegro_gl_set_allegro_mode();

  // Transparent buffer
  rectfill( buffer, 0, 0, SCREEN_W, SCREEN_H, makecol( 255, 0, 255));

  //FPS counter
  if(showFPS){
    textprintf_ex( buffer, font, 0, 0, makecol(0,0,0), makecol(255,255,255), "FPS-%i", fps);
  }

  // Cursor
  draw_sprite( buffer, cursor, mouse_x, mouse_y);

  // Debug text
  textprintf_ex( buffer, font, 0, 15, makecol(0,0,0), makecol(255,255,255), "Camera X:%4.1f Y:%4.1f Z:%4.1f RotX:%4.1f RotY:%4.1f ", gameTiles -> getX(), gameTiles -> getY(), gameTiles -> getZ(), gameTiles -> getRotX(), gameTiles -> getRotY());

  //Draws buffer
  draw_sprite( screen, buffer, 0, 0);

  allegro_gl_unset_allegro_mode();

  allegro_gl_flip();
}

int main( int argc, char* args[]){
  //Setup game
  setup(true);

  //Starts Game
  while(!closeGame){
    //Runs FPS system
    while(ticks == 0){
      //rest(1);
    }
    while(ticks > 0){
      int old_ticks = ticks;
      //Update always
      game();
      ticks--;
      if(old_ticks <= ticks)
        break;
    }
    if(game_time >= old_time + 1){
      fps -= frames_array[frame_index];//decrement the fps by the frames done a second ago
			frames_array[frame_index] = frames_done;//store the number of frames done this 0.1 second
			fps += frames_done;//increment the fps by the newly done frames

			frame_index = (frame_index + 1) % 10;//increment the frame index and snap it to 10

			frames_done = 0;
			old_time += 1;
    }
    //Update every set amount of frames
    draw();
    frames_done++;
  }
  allegro_exit();

  return 0;
}
END_OF_MAIN();
