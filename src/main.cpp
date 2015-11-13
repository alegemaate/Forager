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
volatile int ticks = 0;
int updates_per_second = 100;
volatile int game_time = 0;

int fps;
int frames_done;
int old_time;

void ticker(){
    ticks++;
}
END_OF_FUNCTION(ticker)

void game_time_ticker(){
    game_time++;
}
END_OF_FUNCTION(ticker)

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

void draw();

//Particles
struct particles{
  int x;
  int y;
  bool onScreen;
  int red;
  int blue;
  int green;
};

tile_map* farmTiles;

//Customers
struct customers{
  std::string name;
  int age;
  int preference;
  int speed;
  int experience;
  int gender;

  BITMAP* images[2];

  customers(){
    generate();
  }

  ~customers(){
    delete [] images;
  }

  void generate(){
    std::string firstName;
    std::string lastName;

    switch (random(1,10)){
      case 1 : firstName = "White";
      break;
      case 2 : firstName = "Dog";
      break;
      case 3 : firstName = "Wolf";
      break;
      case 4 : firstName = "Witch's";
      break;
      case 5 : firstName = "Fox";
      break;
      case 6 : firstName = "Green";
      break;
      case 7 : firstName = "Black";
      break;
      case 8 : firstName = "Yellow";
      break;
      case 9 : firstName = "Crab";
      break;
      case 10 : firstName = "Winter";
      break;
    }
    switch (random(1,10)){
      case 1 : lastName = "Broom";
      break;
      case 2 : lastName = "Plant";
      break;
      case 3 : lastName = "Bane";
      break;
      case 4 : lastName = "Wort";
      break;
      case 5 : lastName = "Shade";
      break;
      case 6 : lastName = "Weed";
      break;
      case 7 : lastName = "Cress";
      break;
      case 8 : lastName = "Grass";
      break;
      case 9 : lastName = "Flower";
      break;
      case 10 : lastName = "Rocket";
      break;
    }
    name = firstName + " " + lastName;
    age = random(10,100);
    experience = random(0, 10) * age;
    speed = (experience/random(16, 20)) + 2;
  }
};
customers *farmCustomers[10];

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

    //Creates a buffer
    buffer = create_bitmap( 1280, 960);

    cursor = load_bitmap( "images/cursor.png", NULL);

    //Customers
    for(int i = 0; i < 10; i++){
      farmCustomers[i]  = new customers;
    }

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
    rectfill(buffer, 0, 0, 1280, 960, makecol(0,228,255));

    // Draw map
    farmTiles -> draw( animationFrame);

    //Display customer info
    //if( key[KEY_T])
    //  for(int i=0; i<10; i++)
    //    textprintf_ex(buffer,font,0,100 + i*40,makecol(0,0,0),-1,"Age-%i Gender-%i Name-%s Expierience-%i Speed-%i", farmCustomers[i] -> age, farmCustomers[i] -> gender, farmCustomers[i] -> name.c_str(), farmCustomers[i] -> experience, farmCustomers[i] -> speed);
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
  allegro_init();
  alpng_init();
  install_keyboard();
  install_timer();
  install_mouse();

  set_color_depth(32);

  set_window_title("Forager");

  //Setup game
  setup(true);


  //Starts Game
  while(!closeGame){
    //Runs FPS system
    while(ticks == 0){
      rest(1);
    }
    while(ticks > 0){
      int old_ticks = ticks;
      //Update always
      game();
      ticks--;
      if(old_ticks <= ticks){
        break;
      }
    }
    if(game_time - old_time >= 10){
      fps = frames_done;
      frames_done = 0;
      old_time = game_time;
    }
    //Update every set amount of frames
    draw();
  }

  allegro_exit();

  return 0;
}
END_OF_MAIN();
