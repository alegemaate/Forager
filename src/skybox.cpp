#include "skybox.h"

// Init
skybox::skybox(){

}

// Destory
skybox::~skybox(){

}

// Load the skybox
void skybox::loadSkybox(std::string a_sDirectory, std::string a_sFront, std::string a_sBack, std::string a_sLeft, std::string a_sRight, std::string a_sTop, std::string a_sBottom){
  if(!(texture[0] = load_bitmap( std::string(a_sDirectory + a_sFront).c_str(), NULL)))
    abort_on_error((std::string("Could not load image ") + a_sDirectory + a_sFront).c_str());
  textureRef[0] = allegro_gl_make_texture_ex( AGL_TEXTURE_HAS_ALPHA | AGL_TEXTURE_FLIP, texture[0], GL_RGBA);

  if(!(texture[1] = load_bitmap( std::string(a_sDirectory + a_sBack).c_str(), NULL)))
    abort_on_error((std::string("Could not load image ") + a_sDirectory + a_sBack).c_str());
  textureRef[1] = allegro_gl_make_texture_ex( AGL_TEXTURE_HAS_ALPHA | AGL_TEXTURE_FLIP, texture[1], GL_RGBA);

  if(!(texture[2] = load_bitmap( std::string(a_sDirectory + a_sLeft).c_str(), NULL)))
    abort_on_error((std::string("Could not load image ") + a_sDirectory + a_sLeft).c_str());
  textureRef[2] = allegro_gl_make_texture_ex( AGL_TEXTURE_HAS_ALPHA | AGL_TEXTURE_FLIP, texture[2], GL_RGBA);

  if(!(texture[3] = load_bitmap( std::string(a_sDirectory + a_sRight).c_str(), NULL)))
    abort_on_error((std::string("Could not load image ") + a_sDirectory + a_sRight).c_str());
  textureRef[3] = allegro_gl_make_texture_ex( AGL_TEXTURE_HAS_ALPHA | AGL_TEXTURE_FLIP, texture[3], GL_RGBA);

  if(!(texture[4] = load_bitmap( std::string(a_sDirectory + a_sTop).c_str(), NULL)))
    abort_on_error((std::string("Could not load image ") + a_sDirectory + a_sTop).c_str());
  textureRef[4] = allegro_gl_make_texture_ex( AGL_TEXTURE_HAS_ALPHA | AGL_TEXTURE_FLIP, texture[4], GL_RGBA);

  if(!(texture[5] = load_bitmap( std::string(a_sDirectory + a_sBottom).c_str(), NULL)))
    abort_on_error((std::string("Could not load image ") + a_sDirectory + a_sBottom).c_str());
  textureRef[5] = allegro_gl_make_texture_ex( AGL_TEXTURE_HAS_ALPHA | AGL_TEXTURE_FLIP, texture[5], GL_RGBA);
}

// Render skybox
void skybox::renderSkybox(){
  // Go into modelview matrix
  glPushMatrix();

  // Translate in
  glTranslatef( DEFAULT_MAP_LENGTH/2, DEFAULT_MAP_HEIGHT/2, DEFAULT_MAP_WIDTH/2);

  // FRONT
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureRef[0]);

  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, skyboxSampler);

  glBegin(GL_TRIANGLES);
    glColor4ub(255, 255, 255, 255);
    glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f( -DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH );  //C
    glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f( DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH );  //B
    glNormal3f(0,0,1); glTexCoord2f(1, 0); glVertex3f( -DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //A

    glNormal3f(0,0,1); glTexCoord2f(1, 1); glVertex3f( -DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //C
    glNormal3f(0,0,1); glTexCoord2f(0, 1); glVertex3f( DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //D
    glNormal3f(0,0,1); glTexCoord2f(0, 0); glVertex3f( DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //B
  glEnd();

  // BACK
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureRef[1]);

  glBegin(GL_TRIANGLES);
    glColor4ub(255, 255, 255, 255);
    glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f( DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //H
    glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f( -DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //E
    glNormal3f(0,0,-1); glTexCoord2f(1, 0); glVertex3f( DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //F

    glNormal3f(0,0,-1); glTexCoord2f(1, 1); glVertex3f( DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //H
    glNormal3f(0,0,-1); glTexCoord2f(0, 1); glVertex3f( -DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //G
    glNormal3f(0,0,-1); glTexCoord2f(0, 0); glVertex3f( -DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //E
  glEnd();

  // LEFT
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureRef[2]);

  glBegin(GL_TRIANGLES);
    glColor4ub(255, 255, 255, 255);
    glNormal3f(1,0,0); glTexCoord2f(1, 1); glVertex3f(  -DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //G
    glNormal3f(1,0,0); glTexCoord2f(0, 0); glVertex3f(  -DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //A
    glNormal3f(1,0,0); glTexCoord2f(1, 0); glVertex3f(  -DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //E

    glNormal3f(1,0,0); glTexCoord2f(1, 1); glVertex3f( -DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //G
    glNormal3f(1,0,0); glTexCoord2f(0, 1); glVertex3f( -DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //C
    glNormal3f(1,0,0); glTexCoord2f(0, 0); glVertex3f( -DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //A
  glEnd();

  // RIGHT
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureRef[3]);

  glBegin(GL_TRIANGLES);
    glColor4ub(255, 255, 255, 255);
    glNormal3f(-1,0,0); glTexCoord2f(1, 1); glVertex3f( DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //D
    glNormal3f(-1,0,0); glTexCoord2f(0, 0); glVertex3f( DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //F
    glNormal3f(-1,0,0); glTexCoord2f(1, 0); glVertex3f( DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH );  //B

    glNormal3f(-1,0,0); glTexCoord2f(1, 1); glVertex3f( DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //D
    glNormal3f(-1,0,0); glTexCoord2f(0, 1); glVertex3f( DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //H
    glNormal3f(-1,0,0); glTexCoord2f(0, 0); glVertex3f( DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //F
  glEnd();

  // TOP
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureRef[4]);

  glBegin(GL_TRIANGLES);
    glColor4ub(255, 255, 255, 255);
    glNormal3f(0,-1,0); glTexCoord2f(1, 1); glVertex3f( -DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //G
    glNormal3f(0,-1,0); glTexCoord2f(0, 0); glVertex3f( DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //D
    glNormal3f(0,-1,0); glTexCoord2f(1, 0); glVertex3f( -DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //C

    glNormal3f(0,-1,0); glTexCoord2f(1, 1); glVertex3f(  -DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //G
    glNormal3f(0,-1,0); glTexCoord2f(0, 1); glVertex3f( DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //H
    glNormal3f(0,-1,0); glTexCoord2f(0, 0); glVertex3f( DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //D
  glEnd();

  // BOTTOM
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureRef[5]);

  glBegin(GL_TRIANGLES);
    glColor4ub(255, 255, 255, 255);
    glNormal3f(0,1,0); glTexCoord2f(1, 1); glVertex3f( -DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //A
    glNormal3f(0,1,0); glTexCoord2f(0, 0); glVertex3f( DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //F
    glNormal3f(0,1,0); glTexCoord2f(1, 0); glVertex3f(  -DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //E

    glNormal3f(0,1,0); glTexCoord2f(1, 1); glVertex3f( -DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //A
    glNormal3f(0,1,0); glTexCoord2f(0, 1); glVertex3f( DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH ); //B
    glNormal3f(0,1,0); glTexCoord2f(0, 0); glVertex3f( DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH ); //F
  glEnd();

  // Cool, back to normal
  GLfloat mat_old_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_old_ambient);

  glPopMatrix();
}
