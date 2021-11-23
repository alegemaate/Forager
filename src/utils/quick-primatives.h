/*
  Quick Primitives
  Allan Legemaate
  02/07/16
  An easy to drop in class that can draw primitives or models
*/

#ifndef QUICK_PRIMITIVES_H
#define QUICK_PRIMITIVES_H

#include <alleggl.h>

#include <GL/glu.h>
#include <glm/glm.hpp>

#include "../model.h"

class quick_primitives {
 public:
  static bool load_models();

  static void cube(float primitive_size, float roundness);
  static void cube_flat(float primitive_size);
  static void double_plane();
  static void double_plane_tall();
  static void model_render(float primitive_size, const model& new_model);

  static model test_model;
};

#endif  // QUICK_PRIMITIVES_H
