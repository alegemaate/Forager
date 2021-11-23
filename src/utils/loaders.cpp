#include "loaders.h"

#include "utils.h"

GLuint loaders::loadTexture(const std::string& path) {
  BITMAP* temp = load_bitmap(path.c_str(), nullptr);

  if (!temp) {
    abort_on_error(("Could not load image " + path).c_str());
  }

  return allegro_gl_make_texture_ex(AGL_TEXTURE_HAS_ALPHA | AGL_TEXTURE_FLIP,
                                    temp, GL_RGBA);
}
