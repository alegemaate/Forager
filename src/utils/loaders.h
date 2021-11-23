#ifndef TEXTURE_LOADER_H_
#define TEXTURE_LOADER_H_

#include <alleggl.h>

#include <allegro.h>
#include <string>

namespace loaders {

extern GLuint loadTexture(const std::string& path);

}

#endif  // TEXTURE_LOADER_H_
