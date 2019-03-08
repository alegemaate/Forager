/*
  Quick Primatives
  Allan Legemaate
  02/07/16
  An easy to drop in class that can draw primatives or models
*/

#ifndef QUICK_PRIMATIVES_H
#define QUICK_PRIMATIVES_H

#include <alleggl.h>
#include <GLM/GLM.hpp>
#include <GL/glu.h>
#include <model.h>

class quick_primatives
{
  public:
    quick_primatives();
    virtual ~quick_primatives();

    static bool load_models();

    static void cube( float primative_size, float roundness);
    static void cube_flat( float primative_size);
    static void double_plane( float primative_size);
    static void double_plane_tall( float primative_size);
    static void pyramid( float primative_size);
    static void model_render( float primative_scale, model new_model);

    static model test_model;
    static model model_cube;

  protected:
  private:
};

#endif // QUICK_PRIMATIVES_H
