/*
  Tile Type
  Allan Legemaate
  24/11/15
  Defenitions of the tiles ingame
*/

#ifndef TILE_TYPE_H
#define TILE_TYPE_H

#include <allegro.h>
#include <string>
#include <alleggl.h>
#include <GL/glu.h>

class tile_type{
  public:
    tile_type( std::string newName, unsigned char newType, BITMAP *defaultImage, std::string newModel);
    virtual ~tile_type();

    // Load image from file
    void setImages( std::string path1, std::string path2);

    // Get type
    unsigned short getType(){ return type; }

    // Get name
    std::string getName(){ return name; }

    // Draw
    void draw( BITMAP *tempBuffer, unsigned short x, unsigned short y, unsigned short z, bool newTick);

    // Logic
    void logic();

  protected:
  private:
    unsigned short type;
    std::string name;
    BITMAP *image[2];

    int image_reference_number[2];

    std::string model;
};

#endif // TILE_TYPE_H
