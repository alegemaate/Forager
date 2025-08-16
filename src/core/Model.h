/*
  Model
  Allan Legemaate
  02/07/16
  A nice class that stores vertices, uvs and normals of a model
*/

#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Model {
 public:
  Model();

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;

  GLuint vertex_buffer;
  GLuint normal_buffer;
  GLuint texture_buffer;

  GLuint texture_id;

  bool load(const char* path, const char* uv_path);
};

#endif  // MODEL_H
