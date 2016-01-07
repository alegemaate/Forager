#include "tools.h"

//Iterates through the number of buttons in a joystick and returns true if any keys are pressed
/*bool keyboard_keypressed(){
    bool keypressed=false;
    for(int i=0; i<125; i++)
        if(key[i])keypressed=true;
    return keypressed;

}*/
//Iterates through the number of buttons in a joystick and returns true if any buttons are pressed
/*bool joy_buttonpressed(){
    bool buttonpressed=false;
    for(int i=0; i<joy[0].num_buttons; i++)
        if(joy[0].button[i].b)buttonpressed=true;
    return buttonpressed;
}*/
//Collision
bool collisionAny(int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2){
  if (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1){
    return true;
  }
  return false;
}
bool collisionOverlap(double x1, double width1, double x2, double width2){
  if(x1 - width1 < x2 + width2 && x1 - width1 > x2 - width2){
    return true;
  }
  return false;
}

bool collisionOverlap2(double x1, double width, double x2){
  if(x1 + width > x2 && x1 < x2){
    return true;
  }
  return false;
}


// 3D collision
bool collision3d(double x1, double width1, double x2, double width2, double y1, double height1, double y2, double height2, double z1, double depth1, double z2, double depth2){
  //check the X axis
  if(abs(x1 - x2) < width1 + width2){
    //check the Y axis
    if(abs(y1 - y2) < height1 + height2){
        //check the Z axis
        if(abs(z1 - z2) < depth1 + depth2){
           return true;
        }
    }
  }
  return false;
}

//Checks if file exists
bool fexists(const char *filename){
  std::ifstream ifile(filename);
  return ifile;
}

//Random number generator. Use int random(lowest,highest);
int random(int newLowest, int newHighest){
  if( newHighest < newLowest)
    return -1;

  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest+int(range*rand()/(RAND_MAX + 1.0));
  return randomNumber;
}

//Convert int to string
std::string convertIntToString(int number){
  std::stringstream ss;
  ss << number;
  return ss.str();
}

//Convert double to string
std::string convertDoubleToString(double number){
  std::stringstream ss;
  ss << number;
  return ss.str();
}

//Convert bool to string
std::string convertBoolToString(bool boolean){
  std::stringstream ss;
  ss << boolean;
  return ss.str();
}

// Slope of line
double findSlope( int y_1, int y_2, int x_1, int x_2){
  if( (x_2 - x_1) == 0)
    return 0;
  return (y_2 - y_1)/(x_2 - x_1);
}

// Objects are close
bool closeTo( double val1, double val2, double maxDifference){
  if( abs(val1 - val2) > maxDifference)
    return false;
  return true;
}

// Returns distance 1D
int distanceTo1D( int x1, int x2){
  return abs(x1 - x2);
}

// Returns distance 2D
int distanceTo2D(int x1, int y1, int x2, int y2){
    return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

// Returns distance 2D
int distanceTo3D(int x1, int y1, int z1, int x2, int y2, int z2){
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1));
}

// Divide catch 0
int divideCatchZero( int number1, int number2){
  if( number2 != 0)
    return number1/number2;
  return 0;
}


// false if fully outside, true if inside or intersects
/*bool boxInFrustum( frustum3 const & fru, bound3 const & box )
{
  // check box outside/inside of frustum
  for( int i=0; i<6; i++ ){
      int out = 0;
      out += ((dot( fru.mPlane[i], vec4(box.mMinX, box.mMinY, box.mMinZ, 1.0f) ) < 0.0 )?1:0);
      out += ((dot( fru.mPlane[i], vec4(box.mMaxX, box.mMinY, box.mMinZ, 1.0f) ) < 0.0 )?1:0);
      out += ((dot( fru.mPlane[i], vec4(box.mMinX, box.mMaxY, box.mMinZ, 1.0f) ) < 0.0 )?1:0);
      out += ((dot( fru.mPlane[i], vec4(box.mMaxX, box.mMaxY, box.mMinZ, 1.0f) ) < 0.0 )?1:0);
      out += ((dot( fru.mPlane[i], vec4(box.mMinX, box.mMinY, box.mMaxZ, 1.0f) ) < 0.0 )?1:0);
      out += ((dot( fru.mPlane[i], vec4(box.mMaxX, box.mMinY, box.mMaxZ, 1.0f) ) < 0.0 )?1:0);
      out += ((dot( fru.mPlane[i], vec4(box.mMinX, box.mMaxY, box.mMaxZ, 1.0f) ) < 0.0 )?1:0);
      out += ((dot( fru.mPlane[i], vec4(box.mMaxX, box.mMaxY, box.mMaxZ, 1.0f) ) < 0.0 )?1:0);
      if( out==8 ) return false;
  }

  // check frustum outside/inside box
  int out;
  out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].x > box.mMaxX)?1:0); if( out==8 ) return false;
  out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].x < box.mMinX)?1:0); if( out==8 ) return false;
  out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].y > box.mMaxY)?1:0); if( out==8 ) return false;
  out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].y < box.mMinY)?1:0); if( out==8 ) return false;
  out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].z > box.mMaxZ)?1:0); if( out==8 ) return false;
  out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].z < box.mMinZ)?1:0); if( out==8 ) return false;

  return true;
}*/

// Fade in
/*void highcolor_fade_in(BITMAP* bmp_orig, int speed){
  BITMAP* bmp_buff = create_bitmap(SCREEN_W,SCREEN_H);
  BITMAP* str_orig = create_bitmap( SCREEN_W, SCREEN_H);
  stretch_sprite( str_orig, bmp_orig, 0, 0, SCREEN_W, SCREEN_H);

  if ( speed<=0)
    speed=16;

  for(int a=0; a<256; a+=speed){
    clear( bmp_buff);
    set_trans_blender( 0, 0, 0, a);
    draw_trans_sprite( bmp_buff, str_orig, 0, 0);
    vsync();
    stretch_sprite( screen, bmp_buff, 0, 0,  SCREEN_W, SCREEN_H);
  }
  stretch_sprite( screen, str_orig, 0, 0,  SCREEN_W, SCREEN_H);
}

// Fade out
void highcolor_fade_out(int speed){
  BITMAP* bmp_buff = create_bitmap(SCREEN_W,SCREEN_H);
  BITMAP *bmp_orig = create_bitmap( SCREEN_W, SCREEN_H);
  blit(screen,bmp_orig,0,0,0,0,SCREEN_W,SCREEN_H);

  if ( speed<=0)
    speed=16;

  for( int a=255 - speed; a>0; a-=speed){
    clear( bmp_buff);
    set_trans_blender( 0, 0, 0, a);
    draw_trans_sprite( bmp_buff, bmp_orig, 0, 0);
    vsync();
    stretch_sprite( screen, bmp_buff, 0, 0,  SCREEN_W, SCREEN_H);
  }
  destroy_bitmap( bmp_orig);
  rectfill( screen, 0, 0,  SCREEN_W, SCREEN_H, makecol(0,0,0));
}*/

/*
 *  ERROR REPORTING
 */
void abort_on_error(const char *message){
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}

