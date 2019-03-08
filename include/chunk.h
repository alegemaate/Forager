#ifndef CHUNK_H
#define CHUNK_H

#include <alleggl.h>
#include <GLM/GLM.hpp>

#define CX 16
#define CY 16
#define CZ 16

class chunk
{
  public:
    chunk( int newX, int newY, int newZ);
    virtual ~chunk();

    // Fill array with given data
    void fillArray( glm::vec3 posVec, glm::vec3 normVec, glm::vec2 texVec, GLfloat *newArray, unsigned long index);

    // Tessilate chunk
    void tessellate();

    // Get block
    uint8_t get(int x, int y, int z);

    // Set block
    void set(int x, int y, int z, uint8_t type);

    // Tessilate and such
    void update();

    // Render it all
    void render();


  protected:
  private:
    int index_x, index_y, index_z;

    uint8_t blk[CX][CY][CZ];
    int elements;
    bool changed;

    // Data
    GLuint geometry_array;
    GLuint indice_array;

    unsigned long num_indices;
    unsigned long num_geometry;

    GLfloat *geometry;
    unsigned long *indices;
};

#endif // CHUNK_H
