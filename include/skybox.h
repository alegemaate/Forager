#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include <allegro.h>
#include <alpng.h>
#include <alleggl.h>
#include "ids.h"
#include "tools.h"

class skybox{
  public:
    skybox();
    virtual ~skybox();

    void loadSkybox(std::string a_sDirectory, std::string a_sFront, std::string a_sBack, std::string a_sLeft, std::string a_sRight, std::string a_sTop, std::string a_sBottom);
    void renderSkybox();
  protected:
  private:
    BITMAP* texture[6];
    int textureRef[6];
};

#endif // SKYBOX_H
