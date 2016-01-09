/*
  Chunk
  Allan Legemaate
  08/01/16
  Block of blocks!
*/

#include "chunk.h"

// Construct
chunk::chunk( int newX, int newY, int newZ) {
  // Set pos
  index_x = newX;
  index_y = newY;
  index_z = newZ;

  //memset(blk, 0, sizeof blk);
  changed = true;

  // Set to 0
  geometry_array = 0;
  indice_array = 0;

  // Stores verticies
  num_geometry = CX * CY * CZ * 6 * 6;
  num_indices = num_geometry;

  std::cout << num_geometry << "\n";
  geometry = new GLfloat[8 * num_geometry];
  indices = new unsigned long[num_indices];

  // Make vbo
  glGenBuffers(1, &geometry_array);
  glGenBuffers(1, &indice_array);
}

chunk::~chunk() {
  //glDeleteBuffers(1, &vbo);
}

// Fill array with given data
void chunk::fillArray( glm::vec3 posVec, glm::vec3 normVec, glm::vec2 texVec, GLfloat *newArray, unsigned long index){
  newArray[index] = posVec.x;
  newArray[index + 1] = posVec.y;
  newArray[index + 2] = posVec.z;

  newArray[index + 3] = normVec.x;
  newArray[index + 4] = normVec.y;
  newArray[index + 5] = normVec.z;

  newArray[index + 6] = texVec.x;
  newArray[index + 7] = texVec.y;
}

// Tessilate chunk
void chunk::tessellate(){
  // Vertex counter
  unsigned long j = 0;

  for(int i = 0; i < CX; i++) {
    for(int t = 0; t < CY; t++) {
      for(int k = 0; k < CZ; k++) {
        unsigned char type = 1;//blk[i][k][t];

        // Empty block?
        if(!type)
          continue;

        // View from negative x
        fillArray( glm::vec3( i, t, k), glm::vec3( -1, 0, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i,     t,     k + 1), glm::vec3( -1, 0, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i,     t + 1, k), glm::vec3( -1, 0, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i,     t + 1, k), glm::vec3( -1, 0, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i,     t,     k + 1), glm::vec3( -1, 0, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i,     t + 1, k + 1), glm::vec3( -1, 0, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;

        // View from positive x
        fillArray( glm::vec3(i + 1, t,     k), glm::vec3( 1, 0, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t + 1, k), glm::vec3( 1, 0, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t,     k + 1), glm::vec3( 1, 0, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t + 1, k), glm::vec3( 1, 0, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t + 1, k + 1), glm::vec3( 1, 0, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t    , k + 1), glm::vec3( 1, 0, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;

        // - y
        fillArray( glm::vec3(i, t, k), glm::vec3( 0, -1, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t, k), glm::vec3( 0, -1, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i, t, k + 1), glm::vec3( 0, -1, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t, k), glm::vec3( 0, -1, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t, k + 1), glm::vec3( 0, -1, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i, t, k + 1), glm::vec3( 0, -1, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;

        // + y
        fillArray( glm::vec3(i, t + 1, k), glm::vec3( 0, 1, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i, t + 1, k + 1), glm::vec3( 0, 1, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t + 1, k), glm::vec3( 0, 1, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t + 1, k), glm::vec3( 0, 1, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i, t + 1, k + 1), glm::vec3( 0, 1, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t + 1, k + 1), glm::vec3( 0, 1, 0), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;

        // - z
        fillArray( glm::vec3(i, t, k), glm::vec3( 0, 0, -1), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i, t + 1, k), glm::vec3( 0, 0, -1), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t, k), glm::vec3( 0, 0, -1), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i, t + 1, k), glm::vec3( 0, 0, -1), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t + 1, k), glm::vec3( 0, 0, -1), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t, k), glm::vec3( 0, 0, -1), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;

        // + z
        fillArray( glm::vec3(i, t, k + 1), glm::vec3( 0, 0, 1), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t, k + 1), glm::vec3( 0, 0, 1), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i, t + 1, k + 1), glm::vec3( 0, 0, 1), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i, t + 1, k + 1), glm::vec3( 0, 0, 1), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t, k + 1), glm::vec3( 0, 0, 1), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
        fillArray( glm::vec3(i + 1, t + 1, k + 1), glm::vec3( 0, 0, 1), glm::vec2( 0, 0), geometry, j * 8);
        indices[j] = j;
        j++;
      }
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, geometry_array);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*8*j, NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*8*j, geometry);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indice_array);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned long)*num_indices, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned long)*num_indices, indices);
}

uint8_t chunk::get(int x, int y, int z) {
  return blk[x][y][z];
}

void chunk::set(int x, int y, int z, uint8_t type) {
  blk[x][y][z] = type;
  changed = true;
}

void chunk::update() {
  changed = false;

  // Fill in the VBO here
  tessellate();
}

void chunk::render() {
  // Tessilate when modified
  if(changed)
    update();

  // Go into modelview matrix
  glPushMatrix();

  // Translate in
  glTranslatef( index_x * CX, index_y * CY, index_z * CZ);

  //Render
  // Step 1
  glBindBuffer(GL_ARRAY_BUFFER, geometry_array);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indice_array);

  // Step 2
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  // Step 3
  glTexCoordPointer(3, GL_FLOAT, sizeof(GLfloat)*8, (float*)(sizeof(GLfloat)*5));
  glNormalPointer(GL_FLOAT, sizeof(GLfloat)*8, (float*)(sizeof(GLfloat)*3));
  glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*8, NULL);

  // Step 4
  glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

  // Step 5
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glPopMatrix();
}
