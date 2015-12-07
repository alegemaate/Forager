#include "tile_type.h"

// Init tile
tile_type::tile_type( std::string newName, unsigned char newType, BITMAP *defaultImage){
  // Set init variables
  type = newType;
  name = newName;

  image[0] = defaultImage;
  image[1] = defaultImage;

  image_reference_number = 0;
}

// Destroy tile
tile_type::~tile_type(){
  //dtor
}

// Set images from file
void tile_type::setImages( std::string path1, std::string path2){
  // Load image 1
  if( path1 != "NULL"){
    image[0] = load_bitmap( path1.c_str(), NULL);
    image_reference_number = allegro_gl_make_texture( image[0]);
  }

  // Load image 2
  if( path2 != "NULL"){
    image[1] = load_bitmap( path2.c_str(), NULL);
    image_reference_number = allegro_gl_make_texture( image[1]);
  }
  else{
    image[1] = image[0];
  }
}


// Draw tile
void tile_type::draw( BITMAP *tempBuffer, unsigned short x, unsigned short y, unsigned short z, bool newTick, char zoom, int offsetX, int offsetY){
  // Draw that image if it is onscreen
  /*if(image[newTick] != NULL){
    stretch_sprite( tempBuffer, image[newTick], (x + z) + offsetX/zoom - (image[newTick] -> w / 2 - 64)/zoom, (x - z)/2 - y + offsetY/zoom - (image[newTick] -> h - 128)/zoom, image[newTick]->w/zoom, image[newTick]->h/zoom);
  }*/

  // Enable texturing
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);

  //Define how alpha blending will work and enable alpha blending.
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindTexture(GL_TEXTURE_2D, image_reference_number);

  // No blurr texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Reset Transform
  glLoadIdentity();

  // Translate in
  glTranslatef( x - offsetX, y - offsetY, z -zoom );

  // Rotate when user changes rotate_x and rotate_y
  glRotatef( 45, 1.0, 0.0, 0.0 );
  glRotatef( 45, 0.0, 1.0, 0.0 );

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
}
