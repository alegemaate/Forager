#include "loaders.h"

#include <asw/asw.h>

#include "utils.h"

void loaders::convertSurface(SDL_Surface* surf, GLenum target) {
  SDL_Surface* data = SDL_ConvertSurface(surf, SDL_PIXELFORMAT_RGBA32);
  SDL_DestroySurface(surf);
  if (!data) {
    asw::util::abortOnError("SDL_ConvertSurfaceFormat failed");
  }

  const int bpp = 4;  // ABGR8888 = 4 bytes/px

  // Set pixel storage modes
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, data->pitch / bpp);

  glTexImage2D(target, 0, GL_RGBA8, data->w, data->h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data->pixels);

  // Reset pixel storage modes
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  SDL_DestroySurface(data);
}

GLuint loaders::loadTexture(const std::string& path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  SDL_Surface* surf = IMG_Load(path.c_str());
  if (surf == nullptr) {
    asw::util::abortOnError("Failed to load texture at " + path);
  }

  // Convert to openGL format
  loaders::convertSurface(surf, GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glGenerateMipmap(GL_TEXTURE_2D);

  return textureID;
}

GLuint loaders::loadCubemap(std::vector<std::string> faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  for (unsigned int i = 0; i < faces.size(); i++) {
    SDL_Surface* surf = IMG_Load(faces[i].c_str());
    if (surf == nullptr) {
      asw::util::abortOnError("Failed to load cubemap texture at " + faces[i]);
    }
    loaders::convertSurface(surf, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}
