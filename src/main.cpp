/*
  Forager!
  Allan Legemaate
  04/11/2015
  Foraging Game!
*/

//Includes
#include <allegro.h>
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

//Create variables
int gameScreen = INGAME;
int animationFrame;
int zoom;
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

tile_map* farmTiles;

//Load all ingame content
void setup(bool first){
  zoom = 1;

  showFPS = true;

  //Runs only the first time
  if(first){
    //Set screenmode
    if(false == true){
      resDiv = 1;
      if(set_gfx_mode( GFX_AUTODETECT_FULLSCREEN, 1280, 960, 0, 0) !=0){
        resDiv = 2;
        if(set_gfx_mode( GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0) !=0){
          resDiv = 4;
          if(set_gfx_mode( GFX_AUTODETECT_FULLSCREEN, 320, 240, 0, 0) !=0){
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
          	allegro_message("Unable to go into fullscreen graphic mode\n%s\n", allegro_error);
            exit(1);
          }
        }
      }
    }
    else{
      resDiv = 1;
      if(set_gfx_mode( GFX_AUTODETECT_WINDOWED, 1280, 960, 0, 0) !=0){
        resDiv = 2;
        if(set_gfx_mode( GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) !=0){
          resDiv = 4;
          if(set_gfx_mode( GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0) !=0){
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
          	allegro_message("Unable to set any windowed graphic mode\n%s\n", allegro_error);
            exit(1);
          }
        }
      }
    }

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
      frames_array[ii] = 0;//initialize the array to 0

    //Creates a buffer
    buffer = create_bitmap( 1280, 960);

    cursor = load_bitmap( "images/cursor.png", NULL);

    //normal map
    farmTiles = new tile_map("normal", buffer);
    farmTiles -> load_images();
    farmTiles -> generateMap( "normal");

    //Sets Font
    f1 = load_font("images/fonts/arial_black.pcx", NULL, NULL);
    f2 = extract_font_range(f1, ' ', 'A'-1);
    f3 = extract_font_range(f1, 'A', 'Z');
    f4 = extract_font_range(f1, 'Z'+1, 'z');

    //Merge fonts
    font = merge_fonts(f4, f5 = merge_fonts(f2, f3));
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
      farmTiles -> update();
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
  if(gameScreen == SPLASH){

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
      farmTiles -> draw( animationFrame);
  }

  //FPS counter
  if(showFPS){
    textprintf_ex(buffer,font,0,0,makecol(0,0,0),makecol(255,255,255),"FPS-%i", fps);
  }

  // Cursor
  draw_sprite( buffer, cursor, mouse_x, mouse_y);

  //Draws buffer
  stretch_sprite( screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}

int main(){
  //Initializing
  allegro_init();frames_done++;
  alpng_init();
  install_keyboard();
  install_timer();
  install_mouse();

  set_color_depth(32);

  set_window_title("Forager");

  // Let it run in background
  //set_display_switch_mode(SWITCH_BACKGROUND);

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
