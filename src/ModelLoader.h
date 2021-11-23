/*
  Model Loader
  Allan Legemaate
  03/07/16
  Loads models from file in the OBJ format
*/

#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

class ModelLoader {
 public:
  static bool load_model(const char* path,
                         std::vector<glm::vec3>& out_vertices,
                         std::vector<glm::vec2>& out_uvs,
                         std::vector<glm::vec3>& out_normals);
};

#endif  // MODEL_LOADER_H
