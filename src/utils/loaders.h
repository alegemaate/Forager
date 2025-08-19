#pragma once

#include <GL/glew.h>

#include <GL/gl.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>

namespace loaders {

extern void convertSurface(SDL_Surface* surf, GLenum target);

extern GLuint loadTexture(const std::string& path);

extern GLuint loadCubemap(std::vector<std::string> faces);

}  // namespace loaders
