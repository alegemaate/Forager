

#ifndef FORAGER_GPU_PROGRAM_H
#define FORAGER_GPU_PROGRAM_H

#include <alleggl.h>
#include <glm/glm.hpp>
#include <string>

class GpuProgram {
  unsigned int program_id = 0;
  unsigned int shader_vp = 0;
  unsigned int shader_fp = 0;

 public:
  GpuProgram() = default;

  GpuProgram(const std::string& vsFile, const std::string& fsFile) {
    initFromFile(vsFile, fsFile);
  }

  ~GpuProgram() {
    glDetachShader(program_id, shader_vp);
    glDeleteShader(shader_vp);

    glDetachShader(program_id, shader_fp);
    glDeleteShader(shader_fp);

    glDeleteProgram(program_id);
  }

  void initFromFile(const std::string& vsFile, const std::string& fsFile);

  void init(const std::string& vsText,
            const std::string& fsText,
            const std::string& vsFilename,
            const std::string& fsFilename);

  void activate() const { glUseProgram(program_id); }

  static void deactivate() { glUseProgram(0); }

  __attribute__((unused))
  void setMat4(const std::string& name,
                                       glm::mat4& M) const {
    glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1,
                       GL_TRUE, &M[0][0]);
  }

  __attribute__((unused))
  void setVec3(const std::string& name,
                                       glm::vec3 v) const {
    glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, &v[0]);
  }

  __attribute__((unused))
  void setVec2(const std::string& name,
                                       glm::vec2 v) const {
    glUniform2fv(glGetUniformLocation(program_id, name.c_str()), 1, &v[0]);
  }

  __attribute__((unused))
  void setVec4(const std::string& name,
                                       glm::vec4 v) const {
    glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, &v[0]);
  }

  void setFloat(const std::string& name, float f) const {
    glUniform1f(glGetUniformLocation(program_id, name.c_str()), f);
  }

  void setInt(const std::string& name, int i) const {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), i);
  }

  static std::string textFileRead(const std::string& fileName);
};

#endif  // FORAGER_GPU_PROGRAM_H
