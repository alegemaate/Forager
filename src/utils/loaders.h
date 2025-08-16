#ifndef TEXTURE_LOADER_H_
#define TEXTURE_LOADER_H_

#include <GL/glew.h>

#include <GL/gl.h>
#include <string>
#include <vector>

namespace loaders {

extern GLuint loadTexture(const std::string& path);

extern GLuint loadCubemap(std::vector<std::string> faces);

}  // namespace loaders

#endif  // TEXTURE_LOADER_H_
