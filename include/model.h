/*
  Model
  Allan Legemaate
  02/07/16
  A nice class that stores vertices, uvs and normals of a model
*/

#ifndef MODEL_H
#define MODEL_H

#include <alleggl.h>
#include <vector>
#include <GLM/GLM.hpp>
#include <model_loader.h>
#include "tools.h"

class model
{
  public:
    model();
    virtual ~model();

    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > uvs;
    std::vector< glm::vec3 > normals;

    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint texturebuffer;

    GLuint texture_id;

    bool load( const char * path, const char * uv_path);
  protected:
  private:
};

#endif // MODEL_H
