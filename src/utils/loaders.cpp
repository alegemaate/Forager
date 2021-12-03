#include "loaders.h"

#include "utils.h"

GLuint loaders::loadTexture(const std::string& path) {
  BITMAP* temp = load_bitmap(path.c_str(), nullptr);

  if (!temp) {
    abortOnError("Could not load image " + path);
  }

  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp->w, temp->h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, temp->dat);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glGenerateMipmap(GL_TEXTURE_2D);

  return textureID;
}

BITMAP* loaders::loadImage(const std::string& path) {
  BITMAP* temp = load_bitmap(path.c_str(), nullptr);

  if (!temp) {
    abortOnError("Could not load image " + path);
  }

  return temp;
}
