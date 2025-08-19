

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class GpuProgram {
 public:
  GpuProgram() = default;

  ~GpuProgram() {
    for (const auto& shaderId : shaderIds) {
      glDetachShader(programId, shaderId);
      glDeleteShader(shaderId);
    }

    glDeleteProgram(programId);
  }

  /// Loading APIs
  void initProgram(const std::vector<std::string>& shaders);

  void initProgramFromFiles(const std::vector<std::string>& paths);

  /// Shader management
  void activate() const { glUseProgram(programId); }

  void deactivate() const { glUseProgram(0); }

  /// Value setters
  void setMat4(const std::string& name, const glm::mat4& M) const {
    glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1,
                       GL_FALSE, &M[0][0]);
  }

  void setVec3(const std::string& name, const glm::vec3 v) const {
    glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &v[0]);
  }

  void setVec2(const std::string& name, const glm::vec2 v) const {
    glUniform2fv(glGetUniformLocation(programId, name.c_str()), 1, &v[0]);
  }

  void setVec4(const std::string& name, const glm::vec4 v) const {
    glUniform4fv(glGetUniformLocation(programId, name.c_str()), 1, &v[0]);
  }

  void setFloat(const std::string& name, float f) const {
    glUniform1f(glGetUniformLocation(programId, name.c_str()), f);
  }

  void setInt(const std::string& name, int i) const {
    glUniform1i(glGetUniformLocation(programId, name.c_str()), i);
  }

 private:
  GLuint programId{0};
  std::vector<GLuint> shaderIds;

  static std::string readFromFile(const std::string& path);
  static void validateShader(GLuint shader);
  static void validateProgram(GLuint program);
};
