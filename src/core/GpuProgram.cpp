

#include "GpuProgram.h"

#include <fstream>
#include <iostream>

std::string GpuProgram::textFileRead(const std::string& fileName) {
  std::ifstream file(fileName);
  std::string str;

  file.seekg(0, std::ios::end);
  str.reserve(file.tellg());
  file.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(file)),
             std::istreambuf_iterator<char>());

  return str;
}

static void validateShader(GLuint shader, const std::string& filePath = "") {
  const unsigned int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  GLsizei length = 0;

  glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);

  if (length > 0) {
    std::cout << "Shader " << shader << " (" << filePath
              << ") compile log: " << std::endl
              << buffer << std::endl;
  }
}

static void validateProgram(GLuint program) {
  const unsigned int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  GLsizei length = 0;

  memset(buffer, 0, BUFFER_SIZE);

  glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);

  if (length > 0)
    std::cout << "Program " << program << " link log: " << buffer << std::endl;

  glValidateProgram(program);

  GLint status;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &status);

  if (status == GL_FALSE) {
    std::cout << "Error validating program " << program << std::endl;
  }
}

void GpuProgram::initFromFile(const std::string& vsFile,
                              const std::string& fsFile) {
  std::string vsText = textFileRead(vsFile);

  if (vsText.empty()) {
    std::cerr << "Vertex shader file '" << vsFile << "' not found."
              << std::endl;
    return;
  }

  std::string fsText = textFileRead(fsFile);

  if (fsText.empty()) {
    std::cerr << "Fragment shader file '" << fsFile << "' not found."
              << std::endl;
    return;
  }

  init(vsText, fsText, vsFile, fsFile);
}

void GpuProgram::init(const std::string& vsText,
                      const std::string& fsText,
                      const std::string& vsFilename,
                      const std::string& fsFilename) {
  // Vertex shader
  shader_vp = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(shader_vp, 1, (const char**)&vsText, nullptr);
  glCompileShader(shader_vp);
  validateShader(shader_vp, vsFilename);

  // Fragment shader
  shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shader_fp, 1, (const char**)&fsText, nullptr);
  glCompileShader(shader_fp);
  validateShader(shader_fp, fsFilename);

  // GLSL program
  program_id = glCreateProgram();
  glAttachShader(program_id, shader_vp);
  glAttachShader(program_id, shader_fp);
  glLinkProgram(program_id);
  validateProgram(program_id);
}
