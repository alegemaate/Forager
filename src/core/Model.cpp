#include "Model.h"

#include <cstring>

#include "../utils/loaders.h"
#include "../utils/model-loader.h"
#include "../utils/utils.h"

// Init
Model::Model() {
  // Set to 0
  vertex_buffer = 0;
  normal_buffer = 0;
  texture_buffer = 0;

  texture_id = 0;
}

// Load model
bool Model::load(const char* path, const char* uv_path) {
  if (!loaders::load_model(path, vertices, uvs, normals)) {
    return false;
  }

  // vertices
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
               &vertices[0], GL_STATIC_DRAW);

  // uvs
  glGenBuffers(1, &texture_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0],
               GL_STATIC_DRAW);

  // normals
  glGenBuffers(1, &normal_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0],
               GL_STATIC_DRAW);

  // Image
  // Load image
  if (strcmp(uv_path, "NULL") != 0) {
    texture_id = loaders::loadTexture(uv_path);
  }

  // No blurry textures!
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  return true;
}
