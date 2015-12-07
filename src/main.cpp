/*
  Forager!
  Allan Legemaate
  04/11/2015
  Foraging Game!
*/

//Includes
#include <allegro.h>
#include <alpng.h>
#include <alleggl.h>
#include <GL/glu.h>

#include <string>
#include <time.h>
#include <fstream>
#include <sstream>
#include <math.h>
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

double var1, var2;

//Create fonts
FONT *f1, *f2, *f3, *f4, *f5;

//Create images
BITMAP* buffer;
BITMAP* cursor;
BITMAP* exTexture;

//Resolution X
int resDiv;

//FPS System
int fps;
int frames_done;
int old_time;

// An array to store the number of frames we did during the last 10 tenths of a second
int frames_array[10];
int frame_index = 0;

GLuint texture_number;

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

//Load all ingame content
void setup(bool first){
  showFPS = true;

  //Runs only#include <GL/glut.h> the first time
  if(first){
    /*
     * SOME ALELGRO GL
     */
    //Setting our.. well... settings.  =)  Read the AllegroGL documentation
    //for a more in-depth explanation for what these all do, but they are
    //pretty self explanatory.
    //Note: Z_DEPTH isn't mandatory for 2D drawing.  It's only useful for
    //sorting 3D objects, so when you draw an object behind another, it
    //doesn't get drawn on top of it.  You CAN use it for 2D drawing,
    //but it's REALLY not necessary.
    allegro_gl_set(AGL_Z_DEPTH, 8);
    allegro_gl_set(AGL_COLOR_DEPTH, 32);
    allegro_gl_set(AGL_SUGGEST, AGL_Z_DEPTH | AGL_COLOR_DEPTH);
    allegro_gl_set(AGL_REQUIRE, AGL_DOUBLEBUFFER);

    //Set screenmode
    if(false == true){
      resDiv = 1;
      if(set_gfx_mode( GFX_OPENGL_WINDOWED, 1280, 960, 0, 0) !=0){
        resDiv = 2;
        if(set_gfx_mode( GFX_OPENGL_WINDOWED, 640, 480, 0, 0) !=0){
          resDiv = 4;
          if(set_gfx_mode( GFX_OPENGL_WINDOWED, 320, 240, 0, 0) !=0){
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
          	allegro_message("Unable to go into fullscreen graphic mode\n%s\n", allegro_error);
            exit(1);
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
          	allegro_message("Unable to set any windowed graphic mode\n%s\n", allegro_error);
            exit(1);
          }
        }
      }
    }

    /*
     * SOME OPEN GL
     */
    //I am setting a state where I am editing the projection matrix.
    glMatrixMode(GL_PROJECTION);

    //Clearing the projection matrix...
    glLoadIdentity();

    // set the perspective with the appropriate aspect ratio
    glFrustum(-1, 1, -1, 1, 5, 100);

    //Now editing the model-view matrix.
    glMatrixMode(GL_MODELVIEW);

    //Clearing the model-view matrix.
    glLoadIdentity();

    //glutInitDisplayMode( GLUT_DEPTH );
    glEnable( GL_DEPTH_TEST);

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 0.0, 0.0, 1, 0.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Enable lights
    glEnable(GL_LIGHTING); //turns the "lights" on
    glEnable(GL_LIGHT0); //allows light #0 out of about 8 lights to shine

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

    //Load a texture
    exTexture = load_bitmap( "images/textures/sand.png", NULL);
    texture_number = allegro_gl_make_texture( exTexture);

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
    font = merge_fonts(f4, f5 = merge_fonts(f2, f3));
  }

  var1 = - 5;
  var2 = - 10;
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

  if( key[KEY_O])
    var1 += 0.1;
  if( key[KEY_P])
    var1 -= 0.1;

  if( key[KEY_K])
    var2 += 0.1;
  if( key[KEY_L])
    var2 -= 0.1;

  //Counter for FPS
  frames_done++;
}

//Draw images
void draw(){
  allegro_gl_set_allegro_mode();

  /*if(gameScreen == SPLASH){

  }
  else if(gameScreen == MENU){

  }
  else if(gameScreen == LEVELSELECT){

  }
  else if(gameScreen == INGAME){
    //Background
    rectfill(buffer, 0, 0, 1280, 960, makecol(0,127,255));

    // Draw map
    if( !key[KEY_TILDE])
      gameTiles -> draw( animationFrame);

    // Draw jimmy
    jimmy -> draw( buffer, gameTiles -> getX(), gameTiles -> getY(), gameTiles -> getZoom());
  }

  //FPS counter
  if(showFPS){
    textprintf_ex(buffer,font,0,0,makecol(0,0,0),makecol(255,255,255),"FPS-%i", fps);
  }

  // Cursor
  draw_sprite( buffer, cursor, mouse_x, mouse_y);

  //Draws buffer
  draw_sprite( screen, buffer, 0, 0);

  textprintf_ex(screen,font,0,0,makecol(0,0,0),makecol(255,255,255),"VAR1-%f VAR2-%f", var1, var2);
*/
  allegro_gl_unset_allegro_mode();
  allegro_gl_flip();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw map
  if( !key[KEY_TILDE])
    gameTiles -> draw( animationFrame);

  /*//Enable texturing on all models for now on.
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  //Define how alpha blending will work and enable alpha blending.
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindTexture(GL_TEXTURE_2D, texture_number);
  // No blurr texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Reset Transform
  glLoadIdentity();

  // Texture!
  glBegin(GL_QUADS);
    //If we set the color to white here, then the textured quad won't be
    //tinted red or half-see-through or something when we draw it based on
    //the last call to glColor*().
    glColor4ub(255, 255, 255, 255);

    //Draw our four points, clockwise.
    glTexCoord2f(0, 0); glVertex3f(-1, 1, var2 - 0.3);
    glTexCoord2f(1, 0); glVertex3f(1, 1, var2);
    glTexCoord2f(1, 1); glVertex3f(1, -1, var2 - 0.3);
    glTexCoord2f(0, 1); glVertex3f(-1, -1, var2);
  glEnd();

  // Rotate
  glRotatef(45, 0, 0, 1);

  // No texture :(
  glBegin(GL_QUADS);
    //Define the color (blue)
    glColor3ub(0, 0, 255);

    //Draw our four points, clockwise.
    glVertex3f(-0.5, 0.5, var1 + 0.1);
    glVertex3f(0.5, 0.5, var1);
    glVertex3f(0.5, -0.5, var1);
    glVertex3f(-0.5, -0.5, var1);
  glEnd();*/

/*
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  //Define how alpha blending will work and enable alpha blending.
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindTexture(GL_TEXTURE_2D, texture_number);
  // No blurr texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Reset Transform
  glLoadIdentity();

  // Translate in
  glTranslatef( 0.0, 0.0, -8 );

  // Rotate when user changes rotate_x and rotate_y
  glRotatef( gameTiles -> getX(), 1.0, 0.0, 0.0 );
  glRotatef( gameTiles -> getY(), 0.0, 1.0, 0.0 );

  //Multi-colored side - FRONT
  glBegin(GL_POLYGON);
    glColor4ub(255, 255, 255, 255);
    glTexCoord2f(0, 0);     glVertex3f(  0.5, -0.5, -0.5 );      // P1 is red
    glTexCoord2f(1, 0);     glVertex3f(  0.5,  0.5, -0.5 );      // P2 is green
    glTexCoord2f(1, 1);     glVertex3f( -0.5,  0.5, -0.5 );      // P3 is blue
    glTexCoord2f(0, 1);     glVertex3f( -0.5, -0.5, -0.5 );      // P4 is purple
  glEnd();

  // White side - BACK
  glBegin(GL_POLYGON);
    glColor4ub(255, 255, 255, 255);
    glTexCoord2f(0, 0); glVertex3f(  0.5, -0.5, 0.5 );
    glTexCoord2f(1, 0); glVertex3f(  0.5,  0.5, 0.5 );
    glTexCoord2f(1, 1); glVertex3f( -0.5,  0.5, 0.5 );
    glTexCoord2f(0, 1); glVertex3f( -0.5, -0.5, 0.5 );
  glEnd();

  // Purple side - RIGHT
  glBegin(GL_POLYGON);
    glColor4ub(255, 255, 255, 255);
    glTexCoord2f(0, 0);glVertex3f( 0.5, -0.5, -0.5 );
    glTexCoord2f(1, 0);glVertex3f( 0.5,  0.5, -0.5 );
    glTexCoord2f(1, 1);glVertex3f( 0.5,  0.5,  0.5 );
    glTexCoord2f(0, 1);glVertex3f( 0.5, -0.5,  0.5 );
  glEnd();

  // Green side - LEFT
  glBegin(GL_POLYGON);
    glColor4ub(255, 255, 255, 255);
    glTexCoord2f(0, 0);glVertex3f( -0.5, -0.5,  0.5 );
    glTexCoord2f(1, 0);glVertex3f( -0.5,  0.5,  0.5 );
    glTexCoord2f(1, 1);glVertex3f( -0.5,  0.5, -0.5 );
    glTexCoord2f(0, 1);glVertex3f( -0.5, -0.5, -0.5 );
  glEnd();

  // Blue side - TOP
  glBegin(GL_POLYGON);
    glColor4ub(255, 255, 255, 255);
    glTexCoord2f(0, 0);glVertex3f(  0.5,  0.5,  0.5 );
    glTexCoord2f(1, 0);glVertex3f(  0.5,  0.5, -0.5 );
    glTexCoord2f(1, 1);glVertex3f( -0.5,  0.5, -0.5 );
    glTexCoord2f(0, 1);glVertex3f( -0.5,  0.5,  0.5 );
  glEnd();

  // Red side - BOTTOM
  glBegin(GL_POLYGON);
    glColor4ub(255, 255, 255, 255);
    glTexCoord2f(0, 0);glVertex3f(  0.5, -0.5, -0.5 );
    glTexCoord2f(1, 0);glVertex3f(  0.5, -0.5,  0.5 );
    glTexCoord2f(1, 1);glVertex3f( -0.5, -0.5,  0.5 );
    glTexCoord2f(0, 1);glVertex3f( -0.5, -0.5, -0.5 );
  glEnd();
*/
  glFlush();
}

int main(){
  //Initializing
  allegro_init();
  install_allegro_gl();
  alpng_init();
  install_keyboard();
  install_timer();
  install_mouse();
  set_color_depth( 32);
  set_window_title("Forager");

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
