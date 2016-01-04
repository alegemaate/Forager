#include "tile_type.h"

// Init tile
tile_type::tile_type( std::string newName, unsigned char newType, BITMAP *defaultImage, std::string newModel){
  // Set init variables
  type = newType;
  name = newName;
  model = newModel;

  image[0] = defaultImage;
  image[1] = defaultImage;

  image_reference_number[0] = 0;
  image_reference_number[1] = 0;
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
    image_reference_number[0] = allegro_gl_make_texture_ex( AGL_TEXTURE_HAS_ALPHA | AGL_TEXTURE_FLIP, image[0], GL_RGBA);
  }

  // Load image 2
  if( path2 != "NULL"){
    image[1] = load_bitmap( path2.c_str(), NULL);
    image_reference_number[1] = allegro_gl_make_texture_ex( AGL_TEXTURE_HAS_ALPHA | AGL_TEXTURE_FLIP, image[1], GL_RGBA);
  }
  else{
    image[1] = image[0];
  }
}


// Draw tile
void tile_type::draw( BITMAP *tempBuffer, unsigned short x, unsigned short y, unsigned short z, bool newTick){
  // Tick value
  bool tickVal = newTick;

  if( image_reference_number[1] == 0)
    tickVal = 0;

  if( image_reference_number[tickVal] != 0){
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, image_reference_number[tickVal]);

    // No blurr texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Translate in
    glTranslatef( x, y, z);

    if( model == "MODEL_CUBE" || key[KEY_T]){
      // FRONT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f(  0.5, -0.5, -0.5 );
        glNormal3f(0,0,-1); glTexCoord2f(1, 0); glVertex3f(  0.5,  0.5, -0.5 );
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f( -0.5,  0.5, -0.5 );
        glNormal3f(0,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.5, -0.5, -0.5 );
      glEnd();

      // BACK
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f(  0.5, -0.5, 0.5 );
        glNormal3f(0,0,1); glTexCoord2f(1, 0); glVertex3f(  0.5,  0.5, 0.5 );
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f( -0.5,  0.5, 0.5 );
        glNormal3f(0,0,1); glTexCoord2f(0, 1); glVertex3f( -0.5, -0.5, 0.5 );
      glEnd();

      // RIGHT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(1,0,1); glTexCoord2f(0, 0); glVertex3f( 0.5, -0.5, -0.5 );
        glNormal3f(1,0,1); glTexCoord2f(1, 0); glVertex3f( 0.5,  0.5, -0.5 );
        glNormal3f(1,0,1); glTexCoord2f(1, 1); glVertex3f( 0.5,  0.5,  0.5 );
        glNormal3f(1,0,1); glTexCoord2f(0, 1); glVertex3f( 0.5, -0.5,  0.5 );
      glEnd();

      // LEFT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(-1,0,0); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5,  0.5 );
        glNormal3f(-1,0,0); glTexCoord2f(1, 0); glVertex3f( -0.5,  0.5,  0.5 );
        glNormal3f(-1,0,0); glTexCoord2f(1, 1); glVertex3f( -0.5,  0.5, -0.5 );
        glNormal3f(-1,0,0); glTexCoord2f(0, 1); glVertex3f( -0.5, -0.5, -0.5 );
      glEnd();

      // TOP
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,1,0); glTexCoord2f(0, 0); glVertex3f(  0.5,  0.5,  0.5 );
        glNormal3f(0,1,0); glTexCoord2f(1, 0); glVertex3f(  0.5,  0.5, -0.5 );
        glNormal3f(0,1,0); glTexCoord2f(1, 1); glVertex3f( -0.5,  0.5, -0.5 );
        glNormal3f(0,1,0); glTexCoord2f(0, 1); glVertex3f( -0.5,  0.5,  0.5 );
      glEnd();

      // BOTTOM
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,-1,0); glTexCoord2f(0, 0); glVertex3f(  0.5, -0.5, -0.5 );
        glNormal3f(0,-1,0); glTexCoord2f(1, 0); glVertex3f(  0.5, -0.5,  0.5 );
        glNormal3f(0,-1,0); glTexCoord2f(1, 1); glVertex3f( -0.5, -0.5,  0.5 );
        glNormal3f(0,-1,0); glTexCoord2f(0, 1); glVertex3f( -0.5, -0.5, -0.5 );
      glEnd();
    }
    // Small Cube
    else if( model == "MODEL_CUBE_SMALL"){
      // FRONT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f(  0.25, -0.5, -0.25 );
        glNormal3f(0,0,-1); glTexCoord2f(1, 0); glVertex3f(  0.25,  0.0, -0.25 );
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f( -0.25,  0.0, -0.25 );
        glNormal3f(0,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.25, -0.5, -0.25 );
      glEnd();

      // BACK
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f(  0.25, -0.5, 0.25 );
        glNormal3f(0,0,1); glTexCoord2f(1, 0); glVertex3f(  0.25,  0.0, 0.25 );
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f( -0.25,  0.0, 0.25 );
        glNormal3f(0,0,1); glTexCoord2f(0, 1); glVertex3f( -0.25, -0.5, 0.25 );
      glEnd();

      // RIGHT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(1,0,1); glTexCoord2f(0, 0); glVertex3f( 0.25, -0.5, -0.25 );
        glNormal3f(1,0,1); glTexCoord2f(1, 0); glVertex3f( 0.25,  0.0, -0.25 );
        glNormal3f(1,0,1); glTexCoord2f(1, 1); glVertex3f( 0.25,  0.0,  0.25 );
        glNormal3f(1,0,1); glTexCoord2f(0, 1); glVertex3f( 0.25, -0.5,  0.25 );
      glEnd();

      // LEFT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(-1,0,0); glTexCoord2f(0, 0); glVertex3f( -0.25, -0.5,  0.25 );
        glNormal3f(-1,0,0); glTexCoord2f(1, 0); glVertex3f( -0.25,  0.0,  0.25 );
        glNormal3f(-1,0,0); glTexCoord2f(1, 1); glVertex3f( -0.25,  0.0, -0.25 );
        glNormal3f(-1,0,0); glTexCoord2f(0, 1); glVertex3f( -0.25, -0.5, -0.25 );
      glEnd();

      // TOP
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,1,0); glTexCoord2f(0, 0); glVertex3f(  0.25,  0.0,  0.25 );
        glNormal3f(0,1,0); glTexCoord2f(1, 0); glVertex3f(  0.25,  0.0, -0.25 );
        glNormal3f(0,1,0); glTexCoord2f(1, 1); glVertex3f( -0.25,  0.0, -0.25 );
        glNormal3f(0,1,0); glTexCoord2f(0, 1); glVertex3f( -0.25,  0.0,  0.25 );
      glEnd();

      // BOTTOM
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,-1,0); glTexCoord2f(0, 0); glVertex3f(  0.25, -0.5, -0.25 );
        glNormal3f(0,-1,0); glTexCoord2f(1, 0); glVertex3f(  0.25, -0.5,  0.25 );
        glNormal3f(0,-1,0); glTexCoord2f(1, 1); glVertex3f( -0.25, -0.5,  0.25 );
        glNormal3f(0,-1,0); glTexCoord2f(0, 1); glVertex3f( -0.25, -0.5, -0.25 );
      glEnd();
    }
    // Flat Cube
    else if( model == "MODEL_FLAT"){
      // FRONT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f(  0.5, -0.5, -0.5 );
        glNormal3f(0,0,-1); glTexCoord2f(1, 0); glVertex3f(  0.5, -0.4, -0.5 );
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f( -0.5, -0.4, -0.5 );
        glNormal3f(0,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.5, -0.5, -0.5 );
      glEnd();

      // BACK
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f(  0.5, -0.5, 0.5 );
        glNormal3f(0,0,1); glTexCoord2f(1, 0); glVertex3f(  0.5, -0.4, 0.5 );
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f( -0.5, -0.4, 0.5 );
        glNormal3f(0,0,1); glTexCoord2f(0, 1); glVertex3f( -0.5, -0.5, 0.5 );
      glEnd();

      // RIGHT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(1,0,1); glTexCoord2f(0, 0); glVertex3f( 0.5, -0.5, -0.5 );
        glNormal3f(1,0,1); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.4, -0.5 );
        glNormal3f(1,0,1); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.4,  0.5 );
        glNormal3f(1,0,1); glTexCoord2f(0, 1); glVertex3f( 0.5, -0.5,  0.5 );
      glEnd();

      // LEFT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(-1,0,0); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5,  0.5 );
        glNormal3f(-1,0,0); glTexCoord2f(1, 0); glVertex3f( -0.5, -0.4,  0.5 );
        glNormal3f(-1,0,0); glTexCoord2f(1, 1); glVertex3f( -0.5, -0.4, -0.5 );
        glNormal3f(-1,0,0); glTexCoord2f(0, 1); glVertex3f( -0.5, -0.5, -0.5 );
      glEnd();

      // TOP
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,1,0); glTexCoord2f(0, 0); glVertex3f(  0.5, -0.4,  0.5 );
        glNormal3f(0,1,0); glTexCoord2f(1, 0); glVertex3f(  0.5, -0.4, -0.5 );
        glNormal3f(0,1,0); glTexCoord2f(1, 1); glVertex3f( -0.5, -0.4, -0.5 );
        glNormal3f(0,1,0); glTexCoord2f(0, 1); glVertex3f( -0.5, -0.4,  0.5 );
      glEnd();

      // BOTTOM
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,-1,0); glTexCoord2f(0, 0); glVertex3f(  0.5, -0.5, -0.5 );
        glNormal3f(0,-1,0); glTexCoord2f(1, 0); glVertex3f(  0.5, -0.5,  0.5 );
        glNormal3f(0,-1,0); glTexCoord2f(1, 1); glVertex3f( -0.5, -0.5,  0.5 );
        glNormal3f(0,-1,0); glTexCoord2f(0, 1); glVertex3f( -0.5, -0.5, -0.5 );
      glEnd();
    }
    // X like planes
    else if( model == "MODEL_DOUBLE_PLANE"){
      // FRONT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,-1); glTexCoord2f(1, 0); glVertex3f(  0.25, -0.5, 0.25 );
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f(  0.25,  0.5, 0.25 );
        glNormal3f(0,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.25,  0.5, -0.25 );
        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f( -0.25, -0.5, -0.25 );
      glEnd();

      // FRONT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,1); glTexCoord2f(1, 0); glVertex3f(  0.25, -0.5, 0.25 );
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f(  0.25,  0.5, 0.25 );
        glNormal3f(0,0,1); glTexCoord2f(0, 1); glVertex3f( -0.25,  0.5, -0.25 );
        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f( -0.25, -0.5, -0.25 );
      glEnd();

      // BACK
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,-1); glTexCoord2f(1, 0); glVertex3f(  0.25, -0.5, -0.25 );
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f(  0.25,  0.5, -0.25 );
        glNormal3f(0,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.25,  0.5, 0.25 );
        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f( -0.25, -0.5, 0.25 );
      glEnd();

      // BACK
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,1); glTexCoord2f(1, 0); glVertex3f(  0.25, -0.5, -0.25 );
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f(  0.25,  0.5, -0.25 );
        glNormal3f(0,0,1); glTexCoord2f(0, 1); glVertex3f( -0.25,  0.5, 0.25 );
        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f( -0.25, -0.5, 0.25 );
      glEnd();
    }
    // X like planes tall
    else if( model == "MODEL_DOUBLE_PLANE_TALL"){
      // FRONT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,-1); glTexCoord2f(1, 0); glVertex3f(  0.5, -0.5, 0.5 );
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f(  0.5,  2.5, 0.5 );
        glNormal3f(0,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.5,  2.5, -0.5 );
        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, -0.5 );
      glEnd();

      // BACK
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,1); glTexCoord2f(1, 0); glVertex3f(  0.5, -0.5, -0.5 );
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f(  0.5,  2.5, -0.5 );
        glNormal3f(0,0,1); glTexCoord2f(0, 1); glVertex3f( -0.5,  2.5, 0.5 );
        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, 0.5 );
      glEnd();

      // FRONT
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,1); glTexCoord2f(1, 0); glVertex3f(  0.5, -0.5, 0.5 );
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f(  0.5,  2.5, 0.5 );
        glNormal3f(0,0,1); glTexCoord2f(0, 1); glVertex3f( -0.5,  2.5, -0.5 );
        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, -0.5 );
      glEnd();

      // BACK
      glBegin(GL_POLYGON);
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,-1); glTexCoord2f(1, 0); glVertex3f(  0.5, -0.5, -0.5 );
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f(  0.5,  2.5, -0.5 );
        glNormal3f(0,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.5,  2.5, 0.5 );
        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, 0.5 );
      glEnd();
    }

    glPopMatrix();
  }
}
