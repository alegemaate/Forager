/*
  Chunk
  Allan Legemaate
  08/01/16
  Block of blocks!
*/

#include "Chunk.h"

#include <iostream>
#include "../utils/utils.h"

// Construct
Chunk::Chunk(int x, int y, int z) : index_x(x), index_y(y), index_z(z) {
  // Stores vertices
  num_geometry = CX * CY * CZ * 6 * 6;
  num_indices = num_geometry;

  std::cout << num_geometry << "\n";
  geometry = new GLfloat[9 * num_geometry];
  indices = new unsigned long[num_indices];

  // Make vbo
  glGenBuffers(1, &geometry_array);
  glGenBuffers(1, &indices_array);

  for (int i = 0; i < CX; i++) {
    for (int t = 0; t < CY; t++) {
      for (int k = 0; k < CZ; k++) {
        blk[i][t][k] = random(0, 3);
      }
    }
  }
}

// Fill array with given data
void Chunk::fillArray(glm::vec3 posVec,
                      glm::vec3 normVec,
                      glm::vec2 texVec,
                      GLfloat* newArray,
                      unsigned long index) {
  newArray[index] = posVec.x;
  newArray[index + 1] = posVec.y;
  newArray[index + 2] = posVec.z;

  newArray[index + 3] = normVec.x;
  newArray[index + 4] = normVec.y;
  newArray[index + 5] = normVec.z;

  newArray[index + 6] = texVec.x;
  newArray[index + 7] = texVec.y;
}

// Tessellate chunk
void Chunk::tessellate() {
  // Vertex counter
  unsigned long j = 0;

  for (int i = 0; i < CX; i++) {
    for (int t = 0; t < CY; t++) {
      for (int k = 0; k < CZ; k++) {
        unsigned char type = blk[i][t][k];

        // Empty block?
        if (!type) {
          continue;
        }

        // LEFT (-x)
        if (i == 0 || (i > 0 && blk[i - 1][t][k] == 0)) {
          fillArray(glm::vec3(i - 0.5, t + 0.5, k - 0.5), glm::vec3(-1, 0, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k + 0.5), glm::vec3(-1, 0, 0),
                    glm::vec2(1, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t + 0.5, k + 0.5), glm::vec3(-1, 0, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t + 0.5, k - 0.5), glm::vec3(-1, 0, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k - 0.5), glm::vec3(-1, 0, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k + 0.5), glm::vec3(-1, 0, 0),
                    glm::vec2(0, 1), geometry, j * 9);
          indices[j] = j;
          j++;
        }

        // RIGHT (+x)
        if (i == CX - 1 || (i < CX && blk[i + 1][t][k] == 0)) {
          fillArray(glm::vec3(i + 0.5, t + 0.5, k + 0.5), glm::vec3(1, 0, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;
          fillArray(glm::vec3(i + 0.5, t - 0.5, k + 0.5), glm::vec3(1, 0, 0),
                    glm::vec2(1, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k - 0.5), glm::vec3(1, 0, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k + 0.5), glm::vec3(1, 0, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k - 0.5), glm::vec3(1, 0, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k - 0.5), glm::vec3(1, 0, 0),
                    glm::vec2(0, 1), geometry, j * 9);
          indices[j] = j;
          j++;
        }

        // BOTTOM (-y)
        if (t == 0 || (t > 0 && blk[i][t - 1][k] == 0)) {
          fillArray(glm::vec3(i - 0.5, t - 0.5, k + 0.5), glm::vec3(0, -1, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k - 0.5), glm::vec3(0, -1, 0),
                    glm::vec2(1, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k - 0.5), glm::vec3(0, -1, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k + 0.5), glm::vec3(0, -1, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k - 0.5), glm::vec3(0, -1, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k + 0.5), glm::vec3(0, -1, 0),
                    glm::vec2(0, 1), geometry, j * 9);
          indices[j] = j;
          j++;
        }

        // TOP (+y)
        if (t == CY - 1 || (t < CY && blk[i][t + 1][k] == 0)) {
          fillArray(glm::vec3(i - 0.5, t + 0.5, k - 0.5), glm::vec3(0, 1, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t + 0.5, k + 0.5), glm::vec3(0, 1, 0),
                    glm::vec2(1, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k + 0.5), glm::vec3(0, 1, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t + 0.5, k - 0.5), glm::vec3(0, 1, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k + 0.5), glm::vec3(0, 1, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k - 0.5), glm::vec3(0, 1, 0),
                    glm::vec2(0, 1), geometry, j * 9);
          indices[j] = j;
          j++;
        }

        // BACK(-z)
        if (k == 0 || (k > 0 && blk[i][t][k - 1] == 0)) {
          fillArray(glm::vec3(i + 0.5, t - 0.5, k - 0.5), glm::vec3(0, 0, -1),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k - 0.5), glm::vec3(0, 0, -1),
                    glm::vec2(1, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k - 0.5), glm::vec3(0, 0, -1),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k - 0.5), glm::vec3(0, 0, -1),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k - 0.5), glm::vec3(0, 0, -1),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t + 0.5, k - 0.5), glm::vec3(0, 0, -1),
                    glm::vec2(0, 1), geometry, j * 9);
          indices[j] = j;
          j++;
        }

        // FRONT (+z)
        if (k == CZ - 1 || (k < CZ && blk[i][t][k + 1] == 0)) {
          fillArray(glm::vec3(i - 0.5, t + 0.5, k + 0.5), glm::vec3(0, 0, 1),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k + 0.5), glm::vec3(0, 0, 1),
                    glm::vec2(1, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k + 0.5), glm::vec3(0, 0, 1),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t + 0.5, k + 0.5), glm::vec3(0, 0, 1),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k + 0.5), glm::vec3(0, 0, 1),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k + 0.5), glm::vec3(0, 0, 1),
                    glm::vec2(0, 1), geometry, j * 9);
          indices[j] = j;
          j++;
        }
      }
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, geometry_array);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * j, nullptr,
               GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 9 * j, geometry);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_array);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned long) * num_indices,
               nullptr, GL_STATIC_DRAW);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                  sizeof(unsigned long) * num_indices, indices);
}

uint8_t Chunk::get(int x, int y, int z) {
  return blk[x][y][z];
}

void Chunk::set(int x, int y, int z, uint8_t type) {
  blk[x][y][z] = type;
  changed = true;
}

void Chunk::update() {
  changed = false;

  // Fill in the VBO here
  tessellate();
}

void Chunk::render() {
  // Tessellate when modified
  if (changed) {
    update();
  }

  // Go into model view matrix
  glPushMatrix();

  // Translate in
  glTranslatef(index_x * CX, index_y * CY, index_z * CZ);

  // Render
  //  Step 1
  glBindBuffer(GL_ARRAY_BUFFER, geometry_array);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_array);

  // Step 2
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  // Step 3
  glNormalPointer(GL_FLOAT, sizeof(GLfloat) * 9, (float*)(sizeof(GLfloat) * 3));
  glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 9, nullptr);
  glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 9,
                    (float*)(sizeof(GLfloat) * 6));

  glBindTexture(GL_TEXTURE_2D, 9);

  // Step 4
  glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);

  // Step 5
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  glPopMatrix();
}
