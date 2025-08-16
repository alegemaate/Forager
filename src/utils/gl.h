#pragma once

#include <GL/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>

inline void pick_gl_format(SDL_PixelFormat f,
                           GLint& internalFmt,
                           GLenum& fmt,
                           GLenum& type) {
  type = GL_UNSIGNED_BYTE;
  const SDL_PixelFormatDetails* formatDetails = SDL_GetPixelFormatDetails(f);
  if (formatDetails->bytes_per_pixel == 3) {
    internalFmt = GL_RGB8;
    fmt = (formatDetails->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
  } else {
    internalFmt = GL_RGBA8;
    fmt = (formatDetails->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
  }
}