#ifndef GLSHADER_H
#define GLSHADER_H

#include <allegro.h>
#include <alpng.h>
#include <alleggl.h>
#include <GL/glu.h>

#include <vector>

GLuint LoadShader(const char *vertex_path, const char *fragment_path);

#endif
