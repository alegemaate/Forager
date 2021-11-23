#include "skybox.h"

#include "constants/ids.h"
#include "utils/utils.h"

#include "utils/loaders.h"

// Load the skybox
void skybox::loadSkybox(const std::string& a_sDirectory,
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
}

// Render skybox
void skybox::renderSkybox() {
  // Go into model view matrix
  glPushMatrix();

  // Translate in
  glTranslatef(DEFAULT_MAP_LENGTH / 2.0f, DEFAULT_MAP_HEIGHT / 2.0f,
               DEFAULT_MAP_WIDTH / 2.0f);

  // FRONT
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureRef[0]);

  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, skyboxSampler);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, 0, 1);
  glTexCoord2f(1, 1);
  glVertex3f(-DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // C
  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // B
  glNormal3f(0, 0, 1);
  glTexCoord2f(1, 0);
  glVertex3f(-DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // A

  glNormal3f(0, 0, 1);
  glTexCoord2f(1, 1);
  glVertex3f(-DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // C
  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 1);
  glVertex3f(DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // D
  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // B
  glEnd();

  // BACK
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureRef[1]);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, 0, -1);
  glTexCoord2f(1, 1);
  glVertex3f(DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // H
  glNormal3f(0, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(-DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT,
             -DEFAULT_MAP_WIDTH);  // E
  glNormal3f(0, 0, -1);
  glTexCoord2f(1, 0);
  glVertex3f(DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // F

  glNormal3f(0, 0, -1);
  glTexCoord2f(1, 1);
  glVertex3f(DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // H
  glNormal3f(0, 0, -1);
  glTexCoord2f(0, 1);
  glVertex3f(-DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // G
  glNormal3f(0, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(-DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT,
             -DEFAULT_MAP_WIDTH);  // E
  glEnd();

  // LEFT
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureRef[2]);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 255, 255, 255);
  glNormal3f(1, 0, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // G
  glNormal3f(1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // A
  glNormal3f(1, 0, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT,
             -DEFAULT_MAP_WIDTH);  // E

  glNormal3f(1, 0, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // G
  glNormal3f(1, 0, 0);
  glTexCoord2f(0, 1);
  glVertex3f(-DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // C
  glNormal3f(1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // A
  glEnd();

  // RIGHT
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureRef[3]);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 255, 255, 255);
  glNormal3f(-1, 0, 0);
  glTexCoord2f(1, 1);
  glVertex3f(DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // D
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // F
  glNormal3f(-1, 0, 0);
  glTexCoord2f(1, 0);
  glVertex3f(DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // B

  glNormal3f(-1, 0, 0);
  glTexCoord2f(1, 1);
  glVertex3f(DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // D
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0, 1);
  glVertex3f(DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // H
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // F
  glEnd();

  // TOP
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureRef[4]);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, -1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // G
  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // D
  glNormal3f(0, -1, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // C

  glNormal3f(0, -1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // G
  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 1);
  glVertex3f(DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // H
  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(DEFAULT_MAP_LENGTH, DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // D
  glEnd();

  // BOTTOM
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, textureRef[5]);

  glBegin(GL_TRIANGLES);
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // A
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // F
  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT,
             -DEFAULT_MAP_WIDTH);  // E

  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // A
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 1);
  glVertex3f(DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH);  // B
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(DEFAULT_MAP_LENGTH, -DEFAULT_MAP_HEIGHT, -DEFAULT_MAP_WIDTH);  // F
  glEnd();

  // Cool, back to normal
  GLfloat mat_old_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_old_ambient);

  glPopMatrix();
}
