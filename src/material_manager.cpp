#include "material_manager.h"

int currentMaterial = 0;

// Colours and properties of materials
void changeMaterial( int material){
  // DEFAULT
  if( material == MATERIAL_DEFAULT){
    GLfloat mat_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat mat_diffuse[]  ={ 0.6f, 0.6f, 0.6f, 1.0f};
    GLfloat mat_specular[] = { 0.1f, 0.25f, 0.50f, 1.0f};
    GLfloat mat_shininess[] = { 0.1f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    currentMaterial = MATERIAL_DEFAULT;
  }
  // WATER
  else if( material == MATERIAL_WATER){
    GLfloat mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat mat_diffuse[]  ={ 0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f};
    GLfloat mat_shininess[] = { 0.6f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    currentMaterial = MATERIAL_WATER;
  }
}
