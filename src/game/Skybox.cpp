#include "Skybox.h"

#include "../constants/globals.h"
#include "../utils/utils.h"

#include "../utils/loaders.h"

// Load the skybox
void Skybox::loadSkybox(const std::string& a_sDirectory,
                        const std::string& a_sFront,
                        const std::string& a_sBack,
                        const std::string& a_sLeft,
                        const std::string& a_sRight,
                        const std::string& a_sTop,
                        const std::string& a_sBottom) {
  textureRef[0] = loaders::loadTexture(a_sDirectory + a_sFront);
  textureRef[1] = loaders::loadTexture(a_sDirectory + a_sBack);
  textureRef[2] = loaders::loadTexture(a_sDirectory + a_sLeft);
  textureRef[3] = loaders::loadTexture(a_sDirectory + a_sRight);
  textureRef[4] = loaders::loadTexture(a_sDirectory + a_sTop);
  textureRef[5] = loaders::loadTexture(a_sDirectory + a_sBottom);

  skyboxSampler = loaders::loadTexture("assets/images/skybox/sample.png");
}

// Render skybox
void Skybox::render() {
  skyShader->activate();

  // Go into model view matrix
  glPushMatrix();

  // Bounds
  const int BOUND_X = static_cast<int>(DEFAULT_MAP_WIDTH);
  const int BOUND_Y = static_cast<int>(DEFAULT_MAP_HEIGHT);
  const int BOUND_Z = static_cast<int>(DEFAULT_MAP_LENGTH);

  // Translate in
  glTranslatef(DEFAULT_MAP_LENGTH / 2.0f, BOUND_Y / 2.0f, BOUND_X / 2.0f);

  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, skyboxSampler);

  // FRONT
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureRef[0]);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 255, 255, 255);

  glNormal3f(0, 0, 1);
  glTexCoord2f(1, 1);
  glVertex3f(-BOUND_Z, BOUND_Y, BOUND_X);  // C
  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(BOUND_Z, -BOUND_Y, BOUND_X);  // B
  glNormal3f(0, 0, 1);
  glTexCoord2f(1, 0);
  glVertex3f(-BOUND_Z, -BOUND_Y, BOUND_X);  // A

  glNormal3f(0, 0, 1);
  glTexCoord2f(1, 1);
  glVertex3f(-BOUND_Z, BOUND_Y, BOUND_X);  // C
  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 1);
  glVertex3f(BOUND_Z, BOUND_Y, BOUND_X);  // D
  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(BOUND_Z, -BOUND_Y, BOUND_X);  // B
  glEnd();

  // BACK
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureRef[1]);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, 0, -1);
  glTexCoord2f(1, 1);
  glVertex3f(BOUND_Z, BOUND_Y, -BOUND_X);  // H
  glNormal3f(0, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(-BOUND_Z, -BOUND_Y,
             -BOUND_X);  // E
  glNormal3f(0, 0, -1);
  glTexCoord2f(1, 0);
  glVertex3f(BOUND_Z, -BOUND_Y, -BOUND_X);  // F

  glNormal3f(0, 0, -1);
  glTexCoord2f(1, 1);
  glVertex3f(BOUND_Z, BOUND_Y, -BOUND_X);  // H
  glNormal3f(0, 0, -1);
  glTexCoord2f(0, 1);
  glVertex3f(-BOUND_Z, BOUND_Y, -BOUND_X);  // G
  glNormal3f(0, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(-BOUND_Z, -BOUND_Y,
             -BOUND_X);  // E
  glEnd();

  // LEFT
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureRef[2]);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 255, 255, 255);
  glNormal3f(1, 0, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-BOUND_Z, BOUND_Y, -BOUND_X);  // G
  glNormal3f(1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-BOUND_Z, -BOUND_Y, BOUND_X);  // A
  glNormal3f(1, 0, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-BOUND_Z, -BOUND_Y,
             -BOUND_X);  // E

  glNormal3f(1, 0, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-BOUND_Z, BOUND_Y, -BOUND_X);  // G
  glNormal3f(1, 0, 0);
  glTexCoord2f(0, 1);
  glVertex3f(-BOUND_Z, BOUND_Y, BOUND_X);  // C
  glNormal3f(1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-BOUND_Z, -BOUND_Y, BOUND_X);  // A
  glEnd();

  // RIGHT
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureRef[3]);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 255, 255, 255);
  glNormal3f(-1, 0, 0);
  glTexCoord2f(1, 1);
  glVertex3f(BOUND_Z, BOUND_Y, BOUND_X);  // D
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(BOUND_Z, -BOUND_Y, -BOUND_X);  // F
  glNormal3f(-1, 0, 0);
  glTexCoord2f(1, 0);
  glVertex3f(BOUND_Z, -BOUND_Y, BOUND_X);  // B

  glNormal3f(-1, 0, 0);
  glTexCoord2f(1, 1);
  glVertex3f(BOUND_Z, BOUND_Y, BOUND_X);  // D
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0, 1);
  glVertex3f(BOUND_Z, BOUND_Y, -BOUND_X);  // H
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(BOUND_Z, -BOUND_Y, -BOUND_X);  // F
  glEnd();

  // TOP
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureRef[4]);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, -1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-BOUND_Z, BOUND_Y, -BOUND_X);  // G
  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(BOUND_Z, BOUND_Y, BOUND_X);  // D
  glNormal3f(0, -1, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-BOUND_Z, BOUND_Y, BOUND_X);  // C

  glNormal3f(0, -1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-BOUND_Z, BOUND_Y, -BOUND_X);  // G
  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 1);
  glVertex3f(BOUND_Z, BOUND_Y, -BOUND_X);  // H
  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(BOUND_Z, BOUND_Y, BOUND_X);  // D
  glEnd();

  // BOTTOM
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureRef[5]);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-BOUND_Z, -BOUND_Y, BOUND_X);  // A
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(BOUND_Z, -BOUND_Y, -BOUND_X);  // F
  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-BOUND_Z, -BOUND_Y,
             -BOUND_X);  // E

  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-BOUND_Z, -BOUND_Y, BOUND_X);  // A
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 1);
  glVertex3f(BOUND_Z, -BOUND_Y, BOUND_X);  // B
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(BOUND_Z, -BOUND_Y, -BOUND_X);  // F
  glEnd();

  // Cool, back to normal
  GLfloat mat_old_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_old_ambient);

  glPopMatrix();

  defaultShader->activate();
}
