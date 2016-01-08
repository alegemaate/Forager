#include "tile_type.h"

// Init tile
tile_type::tile_type( std::string newName, unsigned char newType, BITMAP *defaultImage, std::string newModel, unsigned char newAttribute, char newRandomness){
  // Set init variables
  type = newType;
  name = newName;
  model = newModel;

  image[0] = defaultImage;
  image[1] = defaultImage;

  image_reference_number[0] = 0;
  image_reference_number[1] = 0;

  randomness = newRandomness;

  attribute = newAttribute;
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
void tile_type::draw( BITMAP *tempBuffer, double x, double y, double z, bool newTick){
  // Tick value
  bool tickVal = newTick;

  if( image_reference_number[1] == 0)
    tickVal = 0;

  if( image_reference_number[tickVal] != 0){
    // Shader
    if( type == TILE_WATER){
      glUseProgram( waterShader);
      changeMaterial( MATERIAL_WATER);
    }

    glBindTexture(GL_TEXTURE_2D, image_reference_number[tickVal]);

    // No blurr texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Go into modelview matrix
    glPushMatrix();

    // Translate in
    glTranslatef( x, y, z);

    if( model == "MODEL_NONE"){
    }
    else if( model == "MODEL_CUBE" || key[KEY_T]){
      glBegin(GL_TRIANGLES);
        // FRONT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f( -0.5, 0.5, 0.5 );  //C
        glNormal3f(0,0,1); glTexCoord2f(1, 0); glVertex3f( -0.5, -0.5, 0.5 ); //A
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, 0.5 );  //B

        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f( -0.5, 0.5, 0.5 ); //C
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, 0.5 ); //B
        glNormal3f(0,0,1); glTexCoord2f(0, 1); glVertex3f( 0.5, 0.5, 0.5 ); //D

        // BACK
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f( 0.5, 0.5, -0.5 ); //H
        glNormal3f(0,0,-1); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, -0.5 ); //F
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f( -0.5, -0.5, -0.5 ); //E

        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f( 0.5, 0.5, -0.5 ); //H
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f( -0.5, -0.5, -0.5 ); //E
        glNormal3f(0,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.5, 0.5, -0.5 ); //G

        // RIGHT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(1,0,0); glTexCoord2f(0, 0); glVertex3f( 0.5, 0.5, 0.5 ); //D
        glNormal3f(1,0,0); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, 0.5 );  //B
        glNormal3f(1,0,0); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, -0.5 ); //F

        glNormal3f(1,0,0); glTexCoord2f(0, 0); glVertex3f( 0.5, 0.5, 0.5 ); //D
        glNormal3f(1,0,0); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, -0.5 ); //F
        glNormal3f(1,0,0); glTexCoord2f(0, 1); glVertex3f( 0.5, 0.5, -0.5 ); //H

        // LEFT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(-1,0,0); glTexCoord2f(0, 0); glVertex3f(  -0.5, 0.5, -0.5 ); //G
        glNormal3f(-1,0,0); glTexCoord2f(1, 0); glVertex3f(  -0.5, -0.5, -0.5 ); //E
        glNormal3f(-1,0,0); glTexCoord2f(1, 1); glVertex3f(  -0.5, -0.5, 0.5 ); //A

        glNormal3f(-1,0,0); glTexCoord2f(0, 0); glVertex3f( -0.5, 0.5, -0.5 ); //G
        glNormal3f(-1,0,0); glTexCoord2f(1, 1); glVertex3f( -0.5, -0.5, 0.5 ); //A
        glNormal3f(-1,0,0); glTexCoord2f(0, 1); glVertex3f( -0.5, 0.5, 0.5 ); //C

        // BOTTOM
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,-1,0); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, 0.5 ); //A
        glNormal3f(0,-1,0); glTexCoord2f(1, 0); glVertex3f(  -0.5, -0.5, -0.5 ); //E
        glNormal3f(0,-1,0); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, -0.5 ); //F

        glNormal3f(0,-1,0); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, 0.5 ); //A
        glNormal3f(0,-1,0); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, -0.5 ); //F
        glNormal3f(0,-1,0); glTexCoord2f(0, 1); glVertex3f( 0.5, -0.5, 0.5 ); //B

        // TOP
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,1,0); glTexCoord2f(0, 0); glVertex3f(  -0.5, 0.5, -0.5 ); //G
        glNormal3f(0,1,0); glTexCoord2f(1, 0); glVertex3f( -0.5, 0.5, 0.5 ); //C
        glNormal3f(0,1,0); glTexCoord2f(1, 1); glVertex3f( 0.5, 0.5, 0.5 ); //D

        glNormal3f(0,1,0); glTexCoord2f(0, 0); glVertex3f(  -0.5, 0.5, -0.5 ); //G
        glNormal3f(0,1,0); glTexCoord2f(1, 1); glVertex3f( 0.5, 0.5, 0.5 ); //D
        glNormal3f(0,1,0); glTexCoord2f(0, 1); glVertex3f( 0.5, 0.5, -0.5 ); //H
      glEnd();
    }
    // Small Cube
    else if( model == "MODEL_CUBE_SMALL"){
      glBegin(GL_TRIANGLES);
        // FRONT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f( -0.25, 0, 0.25 );  //C
        glNormal3f(0,0,1); glTexCoord2f(1, 0); glVertex3f( -0.25, -0.5, 0.25 ); //A
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f( 0.25, -0.5, 0.25 );  //B

        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f( -0.25, 0, 0.25 ); //C
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f( 0.25, -0.5, 0.25 ); //B
        glNormal3f(0,0,1); glTexCoord2f(0, 1); glVertex3f( 0.25, 0, 0.25 ); //D

        // BACK
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f( 0.25, 0, -0.25 ); //H
        glNormal3f(0,0,-1); glTexCoord2f(1, 0); glVertex3f( 0.25, -0.5, -0.25 ); //F
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f( -0.25, -0.5, -0.25 ); //E

        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f( 0.25, 0, -0.25 ); //H
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f( -0.25, -0.5, -0.25 ); //E
        glNormal3f(0,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.25, 0, -0.25 ); //G

        // RIGHT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(1,0,0); glTexCoord2f(0, 0); glVertex3f( 0.25, 0, 0.25 ); //D
        glNormal3f(1,0,0); glTexCoord2f(1, 0); glVertex3f( 0.25, -0.5, 0.25 );  //B
        glNormal3f(1,0,0); glTexCoord2f(1, 1); glVertex3f( 0.25, -0.5, -0.25 ); //F

        glNormal3f(1,0,0); glTexCoord2f(0, 0); glVertex3f( 0.25, 0, 0.25 ); //D
        glNormal3f(1,0,0); glTexCoord2f(1, 1); glVertex3f( 0.25, -0.5, -0.25 ); //F
        glNormal3f(1,0,0); glTexCoord2f(0, 1); glVertex3f( 0.25, 0, -0.25 ); //H

        // LEFT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(-1,0,0); glTexCoord2f(0, 0); glVertex3f(  -0.25, 0, -0.25 ); //G
        glNormal3f(-1,0,0); glTexCoord2f(1, 0); glVertex3f(  -0.25, -0.5, -0.25 ); //E
        glNormal3f(-1,0,0); glTexCoord2f(1, 1); glVertex3f(  -0.25, -0.5, 0.25 ); //A

        glNormal3f(-1,0,0); glTexCoord2f(0, 0); glVertex3f( -0.25, 0, -0.25 ); //G
        glNormal3f(-1,0,0); glTexCoord2f(1, 1); glVertex3f( -0.25, -0.5, 0.25 ); //A
        glNormal3f(-1,0,0); glTexCoord2f(0, 1); glVertex3f( -0.25, 0, 0.25 ); //C

        // BOTTOM
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,-1,0); glTexCoord2f(0, 0); glVertex3f( -0.25, -0.5, 0.25 ); //A
        glNormal3f(0,-1,0); glTexCoord2f(1, 0); glVertex3f(  -0.25, -0.5, -0.25 ); //E
        glNormal3f(0,-1,0); glTexCoord2f(1, 1); glVertex3f( 0.25, -0.5, -0.25 ); //F

        glNormal3f(0,-1,0); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, 0.25 ); //A
        glNormal3f(0,-1,0); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, -0.25 ); //F
        glNormal3f(0,-1,0); glTexCoord2f(0, 1); glVertex3f( 0.5, -0.5, 0.25 ); //B

        // TOP
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,1,0); glTexCoord2f(0, 0); glVertex3f(  -0.25, 0, -0.25 ); //G
        glNormal3f(0,1,0); glTexCoord2f(1, 0); glVertex3f( -0.25, 0, 0.25 ); //C
        glNormal3f(0,1,0); glTexCoord2f(1, 1); glVertex3f( 0.25, 0, 0.25 ); //D

        glNormal3f(0,1,0); glTexCoord2f(0, 0); glVertex3f(  -0.25, 0, -0.25 ); //G
        glNormal3f(0,1,0); glTexCoord2f(1, 1); glVertex3f( 0.25, 0, 0.25 ); //D
        glNormal3f(0,1,0); glTexCoord2f(0, 1); glVertex3f( 0.25, 0, -0.25 ); //H
      glEnd();
    }
    // Flat Cube
    else if( model == "MODEL_FLAT"){
      glBegin(GL_TRIANGLES);
        // FRONT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.4, 0.5 );  //C
        glNormal3f(0,0,1); glTexCoord2f(1, 0); glVertex3f( -0.5, -0.5, 0.5 ); //A
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, 0.5 );  //B

        glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.4, 0.5 ); //C
        glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, 0.5 ); //B
        glNormal3f(0,0,1); glTexCoord2f(0, 1); glVertex3f( 0.5, -0.4, 0.5 ); //D

        // BACK
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f( 0.5, -0.4, -0.5 ); //H
        glNormal3f(0,0,-1); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, -0.5 ); //F
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f( -0.5, -0.5, -0.5 ); //E

        glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f( 0.5, -0.4, -0.5 ); //H
        glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f( -0.5, -0.5, -0.5 ); //E
        glNormal3f(0,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.5, -0.4, -0.5 ); //G

        // RIGHT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(1,0,0); glTexCoord2f(0, 0); glVertex3f( 0.5, -0.4, 0.5 ); //D
        glNormal3f(1,0,0); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, 0.5 );  //B
        glNormal3f(1,0,0); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, -0.5 ); //F

        glNormal3f(1,0,0); glTexCoord2f(0, 0); glVertex3f( 0.5, -0.4, 0.5 ); //D
        glNormal3f(1,0,0); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, -0.5 ); //F
        glNormal3f(1,0,0); glTexCoord2f(0, 1); glVertex3f( 0.5, -0.4, -0.5 ); //H

        // LEFT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(-1,0,0); glTexCoord2f(0, 0); glVertex3f(  -0.5, -0.4, -0.5 ); //G
        glNormal3f(-1,0,0); glTexCoord2f(1, 0); glVertex3f(  -0.5, -0.5, -0.5 ); //E
        glNormal3f(-1,0,0); glTexCoord2f(1, 1); glVertex3f(  -0.5, -0.5, 0.5 ); //A

        glNormal3f(-1,0,0); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.4, -0.5 ); //G
        glNormal3f(-1,0,0); glTexCoord2f(1, 1); glVertex3f( -0.5, -0.5, 0.5 ); //A
        glNormal3f(-1,0,0); glTexCoord2f(0, 1); glVertex3f( -0.5, -0.4, 0.5 ); //C

        // BOTTOM
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,-1,0); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, 0.5 ); //A
        glNormal3f(0,-1,0); glTexCoord2f(1, 0); glVertex3f(  -0.5, -0.5, -0.5 ); //E
        glNormal3f(0,-1,0); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, -0.5 ); //F

        glNormal3f(0,-1,0); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, 0.5 ); //A
        glNormal3f(0,-1,0); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.5, -0.5 ); //F
        glNormal3f(0,-1,0); glTexCoord2f(0, 1); glVertex3f( 0.5, -0.5, 0.5 ); //B

        // TOP
        glColor4ub(255, 255, 255, 255);
        glNormal3f(0,1,0); glTexCoord2f(0, 0); glVertex3f(  -0.5, -0.4, -0.5 ); //G
        glNormal3f(0,1,0); glTexCoord2f(1, 0); glVertex3f( -0.5, -0.4, 0.5 ); //C
        glNormal3f(0,1,0); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.4, 0.5 ); //D

        glNormal3f(0,1,0); glTexCoord2f(0, 0); glVertex3f(  -0.5, -0.4, -0.5 ); //G
        glNormal3f(0,1,0); glTexCoord2f(1, 1); glVertex3f( 0.5, -0.4, 0.5 ); //D
        glNormal3f(0,1,0); glTexCoord2f(0, 1); glVertex3f( 0.5, -0.4, -0.5 ); //H
      glEnd();
    }
    // X like planes
    else if( model == "MODEL_DOUBLE_PLANE"){
      glBegin(GL_TRIANGLES);
        // SIDE 1 FRONT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(1,0,1); glTexCoord2f(0, 1); glVertex3f( -0.25, 0.5, 0.25 );  //C
        glNormal3f(1,0,1); glTexCoord2f(0, 0); glVertex3f( -0.25, -0.5, 0.25 ); //A
        glNormal3f(1,0,1); glTexCoord2f(1, 0); glVertex3f( 0.25, -0.5, -0.25 );  //B

        glNormal3f(1,0,1); glTexCoord2f(0, 1); glVertex3f( -0.25, 0.5, 0.25 ); //C
        glNormal3f(1,0,1); glTexCoord2f(1, 0); glVertex3f( 0.25, -0.5, -0.25 ); //B
        glNormal3f(1,0,1); glTexCoord2f(1, 1); glVertex3f( 0.25, 0.5, -0.25 ); //D

        // SIDE 1 BACK
        glColor4ub(255, 255, 255, 255);
        glNormal3f(-1,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.25, 0.5, 0.25 );  //C
        glNormal3f(-1,0,-1); glTexCoord2f(1, 0); glVertex3f( 0.25, -0.5, -0.25 );  //B
        glNormal3f(-1,0,-1); glTexCoord2f(0, 0); glVertex3f( -0.25, -0.5, 0.25 ); //A

        glNormal3f(-1,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.25, 0.5, 0.25 ); //C
        glNormal3f(-1,0,-1); glTexCoord2f(1, 1); glVertex3f( 0.25, 0.5, -0.25 ); //D
        glNormal3f(-1,0,-1); glTexCoord2f(1, 0); glVertex3f( 0.25, -0.5, -0.25 ); //B


        // SIDE 2 FRONT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(-1,0,1); glTexCoord2f(0, 1); glVertex3f( -0.25, 0.5, -0.25 );  //C
        glNormal3f(-1,0,1); glTexCoord2f(0, 0); glVertex3f( -0.25, -0.5, -0.25 ); //A
        glNormal3f(-1,0,1); glTexCoord2f(1, 0); glVertex3f( 0.25, -0.5, 0.25 );  //B

        glNormal3f(-1,0,1); glTexCoord2f(0, 1); glVertex3f( -0.25, 0.5, -0.25 ); //C
        glNormal3f(-1,0,1); glTexCoord2f(1, 0); glVertex3f( 0.25, -0.5, 0.25 ); //B
        glNormal3f(-1,0,1); glTexCoord2f(1, 1); glVertex3f( 0.25, 0.5, 0.25 ); //D

        // SIDE 2 BACK
        glColor4ub(255, 255, 255, 255);
        glNormal3f(1,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.25, 0.5, -0.25 );  //C
        glNormal3f(1,0,-1); glTexCoord2f(1, 0); glVertex3f( 0.25, -0.5, 0.25 );  //B
        glNormal3f(1,0,-1); glTexCoord2f(0, 0); glVertex3f( -0.25, -0.5, -0.25 ); //A

        glNormal3f(1,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.25, 0.5, -0.25 ); //C
        glNormal3f(1,0,-1); glTexCoord2f(1, 1); glVertex3f( 0.25, 0.5, 0.25 ); //D
        glNormal3f(1,0,-1); glTexCoord2f(1, 0); glVertex3f( 0.25, -0.5, 0.25 ); //B
      glEnd();
    }
    // X like planes tall
    else if( model == "MODEL_DOUBLE_PLANE_TALL"){
      glBegin(GL_TRIANGLES);
        // SIDE 1 FRONT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(1,0,1); glTexCoord2f(0, 1); glVertex3f( -0.5, 2.5, 0.5 );  //C
        glNormal3f(1,0,1); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, 0.5 ); //A
        glNormal3f(1,0,1); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, -0.5 );  //B

        glNormal3f(1,0,1); glTexCoord2f(0, 1); glVertex3f( -0.5, 2.5, 0.5 ); //C
        glNormal3f(1,0,1); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, -0.5 ); //B
        glNormal3f(1,0,1); glTexCoord2f(1, 1); glVertex3f( 0.5, 2.5, -0.5 ); //D

        // SIDE 1 BACK
        glColor4ub(255, 255, 255, 255);
        glNormal3f(-1,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.5, 2.5, 0.5 );  //C
        glNormal3f(-1,0,-1); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, -0.5 );  //B
        glNormal3f(-1,0,-1); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, 0.5 ); //A

        glNormal3f(-1,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.5, 2.5, 0.5 ); //C
        glNormal3f(-1,0,-1); glTexCoord2f(1, 1); glVertex3f( 0.5, 2.5, -0.5 ); //D
        glNormal3f(-1,0,-1); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, -0.5 ); //B


        // SIDE 2 FRONT
        glColor4ub(255, 255, 255, 255);
        glNormal3f(-1,0,1); glTexCoord2f(0, 1); glVertex3f( -0.5, 2.5, -0.5 );  //C
        glNormal3f(-1,0,1); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, -0.5 ); //A
        glNormal3f(-1,0,1); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, 0.5 );  //B

        glNormal3f(-1,0,1); glTexCoord2f(0, 1); glVertex3f( -0.5, 2.5, -0.5 ); //C
        glNormal3f(-1,0,1); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, 0.5 ); //B
        glNormal3f(-1,0,1); glTexCoord2f(1, 1); glVertex3f( 0.5, 2.5, 0.5 ); //D

        // SIDE 2 BACK
        glColor4ub(255, 255, 255, 255);
        glNormal3f(1,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.5, 2.5, -0.5 );  //C
        glNormal3f(1,0,-1); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, 0.5 );  //B
        glNormal3f(1,0,-1); glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, -0.5 ); //A

        glNormal3f(1,0,-1); glTexCoord2f(0, 1); glVertex3f( -0.5, 2.5, -0.5 ); //C
        glNormal3f(1,0,-1); glTexCoord2f(1, 1); glVertex3f( 0.5, 2.5, 0.5 ); //D
        glNormal3f(1,0,-1); glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, 0.5 ); //B
      glEnd();
    }

    glPopMatrix();

    // Shader back
    if( currentMaterial != MATERIAL_DEFAULT){
      changeMaterial( MATERIAL_DEFAULT);
      glUseProgram( defaultShader);
    }
  }
}
