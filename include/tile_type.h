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

#include "globals.h"
#include "ids.h"
#include "material_manager.h"
#include "quick_primatives.h"

class tile_type{
  public:
    tile_type( std::string newName, unsigned char newType, BITMAP *defaultImage, std::string newModel, unsigned char newAttribute = 0, char newRandomness = 0);
    virtual ~tile_type();

    // Load image from file
    void setImages( std::string path1, std::string path2);

    // Get type
    char getType(){ return type; }

    // Get name
    std::string getName(){ return name; }

    // Get model
    std::string getModel(){ return model; }

    // Get model
    unsigned char getRandomness(){ return randomness; }

    // Get type
    unsigned char getAttribute(){ return attribute; }

    // Tex
    int getImageRef(){ return image_reference_number[0]; }

    // Draw
    void draw( double x, double y, double z, bool newTick);

    // Logic
    void logic();

  protected:
  private:
    unsigned char type;
    std::string name;
    BITMAP *image[2];
    unsigned char attribute;
    char randomness;

    int image_reference_number[2];

    std::string model;
};

#endif // TILE_TYPE_H
