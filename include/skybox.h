/*
  Skybox
  Allan Legemaate
  04/01/16
  A neato lil skybox loader
*/

#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include <allegro.h>
#include <alpng.h>
#include <alleggl.h>
#include "ids.h"
#include "tools.h"
#include "globals.h"

class skybox{
  public:
    skybox();
    virtual ~skybox();

    void loadSkybox(std::string a_sDirectory, std::string a_sFront, std::string a_sBack, std::string a_sLeft, std::string a_sRight, std::string a_sTop, std::string a_sBottom);
    void renderSkybox();

    int skyboxSampler;
  protected:
  private:
    BITMAP *texture[6];
    int textureRef[6];
};

#endif // SKYBOX_H