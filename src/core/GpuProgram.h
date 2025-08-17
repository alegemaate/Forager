

#ifndef FORAGER_GPU_PROGRAM_H
#define FORAGER_GPU_PROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class GpuProgram {
  unsigned int id = 0;
  unsigned int vertex = 0;
  unsigned int fragment = 0;

 public:
  GpuProgram() = default;

  GpuProgram(const std::string& vsFile, const std::string& fsFile) {
    initFromFile(vsFile, fsFile);
  }

  ~GpuProgram() {
    glDetachShader(id, vertex);
    glDeleteShader(vertex);

    glDetachShader(id, fragment);
    glDeleteShader(fragment);

    glDeleteProgram(id);
  }

  void initFromFile(const std::string& vsFile, const std::string& fsFile);

  void init(const std::string& vsText,
            const std::string& fsText,
            const std::string& vsFilename,
            const std::string& fsFilename);

  void activate() const { glUseProgram(id); }

  static void deactivate() { glUseProgram(0); }

  void setMat4(const std::string& name, const glm::mat4& M) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                       &M[0][0]);
  }

  void setVec3(const std::string& name, const glm::vec3 v) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &v[0]);
  }

  void setVec2(const std::string& name, const glm::vec2 v) const {
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &v[0]);
  }

  void setVec4(const std::string& name, const glm::vec4 v) const {
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &v[0]);
  }

  void setFloat(const std::string& name, float f) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), f);
  }

  void setInt(const std::string& name, int i) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), i);
  }

  GLint getUniformLocation(const std::string& name) const {
    return glGetUniformLocation(id, name.c_str());
  }

  static std::string textFileRead(const std::string& fileName);
};

#endif  // FORAGER_GPU_PROGRAM_H
