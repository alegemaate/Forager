#ifndef UTILS_SHADER_H_
#define UTILS_SHADER_H_

#include <allegro.h>

#include <alleggl.h>
#include <string>

extern void setupProgram(GLuint newProgram);

extern void setupShader(const std::string& shaderFile, GLuint newShader);

#endif  // UTILS_SHADER_H_
