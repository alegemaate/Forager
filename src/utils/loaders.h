#ifndef TEXTURE_LOADER_H_
#define TEXTURE_LOADER_H_

#include <alleggl.h>

#include <allegro.h>
#include <string>

namespace loaders {

extern GLuint loadTexture(const std::string& path);

extern BITMAP* loadImage(const std::string& path);

}  // namespace loaders

#endif  // TEXTURE_LOADER_H_
