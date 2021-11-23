/*
  Tile
  Allan Legemaate
  11/11/15
  Class for the tile data (for images see TextureLoader.h)
*/

#ifndef TILE_H
#define TILE_H

#include <allegro.h>

#include "TileType.h"

class Tile {
 public:
  Tile(unsigned char x, unsigned char y, unsigned char z, TileType* type);

  unsigned short getType() { return tileImpl->getType(); }
  TileType* getTile() { return tileImpl; }

  void setType(TileType* val);
  void draw(int newTick);

  // Get X value
  unsigned char getX() const { return x; }
  unsigned char getY() const { return y; }
  unsigned char getZ() const { return z; }

  void setX(unsigned char newVal) { x = newVal; }
  void setY(unsigned char newVal) { y = newVal; }
  void setZ(unsigned char newVal) { z = newVal; }

  void jiggle(char xJiggle, char yJiggle, char zJiggle);

  // Set/Get biome
  void setBiome(int id) { biome = id; }
  unsigned char getBiome() const { return biome; }

 private:
  unsigned char x;
  unsigned char y;
  unsigned char z;

  char xJiggle = 0;
  char yJiggle = 0;
  char zJiggle = 0;

  unsigned char biome = 0;

  TileType* tileImpl = nullptr;
};

#endif  // TILE_H
