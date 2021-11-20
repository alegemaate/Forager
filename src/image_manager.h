/*
  Image Manager
  Allan Legemaate
  16/11/15
  Loads all images for the tiles and sets them to a corresponding tile ID
*/
#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#define NUMBER_IMAGES 128

#include <allegro.h>

class image_manager
{
  public:
    image_manager();
    virtual ~image_manager();
  protected:
  private:
    BITMAP *images[NUMBER_IMAGES];
};

#endif // IMAGE_MANAGER_H
