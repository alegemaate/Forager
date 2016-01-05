/*
  Material Manager
  Allan Legemaate
  05/01/16
  Just a function to quickly change materials
*/

#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <allegro.h>
#include <alleggl.h>
#include <iostream>

#define MATERIAL_DEFAULT 0
#define MATERIAL_WATER 1
#define MATERIAL_COPPER 2
#define MATERIAL_RUBBER 3

extern void changeMaterial( int material);

extern int currentMaterial;

#endif // MATERIAL_MANAGER_H
