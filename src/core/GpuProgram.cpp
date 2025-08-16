

#include "GpuProgram.h"

#include <fstream>
#include "../core/Logger.h"
#include "../utils/utils.h"

std::string GpuProgram::textFileRead(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    abortOnError("Cannot find file " + path +
                 " \n Please check your files and try again");
  }

  std::string str;

  file.seekg(0, std::ios::end);
  str.reserve(file.tellg());
  file.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(file)),
             std::istreambuf_iterator<char>());

  return str;
}

static void validateShader(GLuint shader, const std::string& filePath = "") {
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    const unsigned int BUFFER_SIZE = 1024;
    GLchar infoLog[BUFFER_SIZE];
    glGetShaderInfoLog(shader, BUFFER_SIZE, nullptr, infoLog);

    Logger::log("Shader " + std::to_string(shader) + " (" + filePath +
                ") compile log: \n" + infoLog);
  }
}

static void validateProgram(GLuint program) {
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    const unsigned int BUFFER_SIZE = 1024;
    GLchar infoLog[BUFFER_SIZE];
    glGetProgramInfoLog(program, BUFFER_SIZE, nullptr, infoLog);

    Logger::log("Program " + std::to_string(program) + " compile log: \n" +
                infoLog);
  }
}

void GpuProgram::initFromFile(const std::string& vsFile,
                              const std::string& fsFile) {
  std::string vsText = textFileRead(vsFile);

  if (vsText.empty()) {
    Logger::log("Vertex shader file '" + vsFile + "' not found.");
    return;
  }

  std::string fsText = textFileRead(fsFile);

  if (fsText.empty()) {
    Logger::log("Fragment shader file '" + fsFile + "' not found.");

    return;
  }

  init(vsText, fsText, vsFile, fsFile);
}

void GpuProgram::init(const std::string& vsText,
                      const std::string& fsText,
                      const std::string& vsFilename,
                      const std::string& fsFilename) {
  // Vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, (const char**)&vsText, nullptr);
  glCompileShader(vertex);
  validateShader(vertex, vsFilename);

  // Fragment shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, (const char**)&fsText, nullptr);
  glCompileShader(fragment);
  validateShader(fragment, fsFilename);

  // GLSL program
  id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);
  validateProgram(id);
}
