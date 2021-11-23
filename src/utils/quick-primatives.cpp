#include "quick-primatives.h"

model quick_primitives::test_model;

bool quick_primitives::load_models() {
  return test_model.load("models/rock.obj", "models/rock_uv.png");
}

// Resizable Cube
void quick_primitives::cube(float primitive_size, float roundness) {
  // Size / 2
  float temp_size = (primitive_size / 2);

  float temp_round_top = roundness;
  float temp_round_bottom = 1 - roundness;

  glBegin(GL_TRIANGLES);
  // FRONT
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, temp_round_top, 1);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, temp_size, temp_size);  // C
  glNormal3f(0, temp_round_bottom, 1);
  glTexCoord2f(1, 0);
  glVertex3f(-temp_size, -temp_size, temp_size);  // A
  glNormal3f(0, temp_round_bottom, 1);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -temp_size, temp_size);  // B

  glNormal3f(0, temp_round_top, 1);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, temp_size, temp_size);  // C
  glNormal3f(0, temp_round_bottom, 1);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -temp_size, temp_size);  // B
  glNormal3f(0, temp_round_top, 1);
  glTexCoord2f(0, 1);
  glVertex3f(temp_size, temp_size, temp_size);  // D

  // BACK
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, temp_round_top, -1);
  glTexCoord2f(0, 0);
  glVertex3f(temp_size, temp_size, -temp_size);  // H
  glNormal3f(0, temp_round_bottom, -1);
  glTexCoord2f(1, 0);
  glVertex3f(temp_size, -temp_size, -temp_size);  // F
  glNormal3f(0, temp_round_bottom, -1);
  glTexCoord2f(1, 1);
  glVertex3f(-temp_size, -temp_size, -temp_size);  // E

  glNormal3f(0, temp_round_top, -1);
  glTexCoord2f(0, 0);
  glVertex3f(temp_size, temp_size, -temp_size);  // H
  glNormal3f(0, temp_round_bottom, -1);
  glTexCoord2f(1, 1);
  glVertex3f(-temp_size, -temp_size, -temp_size);  // E
  glNormal3f(0, temp_round_top, -1);
  glTexCoord2f(0, 1);
  glVertex3f(-temp_size, temp_size, -temp_size);  // G

  // RIGHT
  glColor4ub(255, 255, 255, 255);
  glNormal3f(1, temp_round_top, 0);
  glTexCoord2f(0, 0);
  glVertex3f(temp_size, temp_size, temp_size);  // D
  glNormal3f(1, temp_round_bottom, 0);
  glTexCoord2f(1, 0);
  glVertex3f(temp_size, -temp_size, temp_size);  // B
  glNormal3f(1, temp_round_bottom, 0);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -temp_size, -temp_size);  // F

  glNormal3f(1, temp_round_top, 0);
  glTexCoord2f(0, 0);
  glVertex3f(temp_size, temp_size, temp_size);  // D
  glNormal3f(1, temp_round_bottom, 0);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -temp_size, -temp_size);  // F
  glNormal3f(1, temp_round_top, 0);
  glTexCoord2f(0, 1);
  glVertex3f(temp_size, temp_size, -temp_size);  // H

  // LEFT
  glColor4ub(255, 255, 255, 255);
  glNormal3f(-1, temp_round_top, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, temp_size, -temp_size);  // G
  glNormal3f(-1, temp_round_bottom, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-temp_size, -temp_size, -temp_size);  // E
  glNormal3f(-1, temp_round_bottom, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-temp_size, -temp_size, temp_size);  // A

  glNormal3f(-1, temp_round_top, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, temp_size, -temp_size);  // G
  glNormal3f(-1, temp_round_bottom, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-temp_size, -temp_size, temp_size);  // A
  glNormal3f(-1, temp_round_top, 0);
  glTexCoord2f(0, 1);
  glVertex3f(-temp_size, temp_size, temp_size);  // C

  // BOTTOM
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, -temp_size, temp_size);  // A
  glNormal3f(0, -1, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-temp_size, -temp_size, -temp_size);  // E
  glNormal3f(0, -1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -temp_size, -temp_size);  // F

  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, -temp_size, temp_size);  // A
  glNormal3f(0, -1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -temp_size, -temp_size);  // F
  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 1);
  glVertex3f(temp_size, -temp_size, temp_size);  // B

  // TOP
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, temp_size, -temp_size);  // G
  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-temp_size, temp_size, temp_size);  // C
  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, temp_size, temp_size);  // D

  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, temp_size, -temp_size);  // G
  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, temp_size, temp_size);  // D
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 1);
  glVertex3f(temp_size, temp_size, -temp_size);  // H
  glEnd();
}

// Flat cube
void quick_primitives::cube_flat(float primitive_size) {
  // Size / 2
  float temp_size = (primitive_size / 2);

  glBegin(GL_TRIANGLES);
  // FRONT
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, -(temp_size - 0.1f), temp_size);  // C
  glNormal3f(0, 0, 1);
  glTexCoord2f(1, 0);
  glVertex3f(-temp_size, -temp_size, temp_size);  // A
  glNormal3f(0, 0, 1);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -temp_size, temp_size);  // B

  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, -(temp_size - 0.1f), temp_size);  // C
  glNormal3f(0, 0, 1);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -temp_size, temp_size);  // B
  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 1);
  glVertex3f(temp_size, -(temp_size - 0.1f), temp_size);  // D

  // BACK
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(temp_size, -(temp_size - 0.1f), -temp_size);  // H
  glNormal3f(0, 0, -1);
  glTexCoord2f(1, 0);
  glVertex3f(temp_size, -temp_size, -temp_size);  // F
  glNormal3f(0, 0, -1);
  glTexCoord2f(1, 1);
  glVertex3f(-temp_size, -temp_size, -temp_size);  // E

  glNormal3f(0, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(temp_size, -(temp_size - 0.1f), -temp_size);  // H
  glNormal3f(0, 0, -1);
  glTexCoord2f(1, 1);
  glVertex3f(-temp_size, -temp_size, -temp_size);  // E
  glNormal3f(0, 0, -1);
  glTexCoord2f(0, 1);
  glVertex3f(-temp_size, -(temp_size - 0.1f), -temp_size);  // G

  // RIGHT
  glColor4ub(255, 255, 255, 255);
  glNormal3f(1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(temp_size, -(temp_size - 0.1f), temp_size);  // D
  glNormal3f(1, 0, 0);
  glTexCoord2f(1, 0);
  glVertex3f(temp_size, -temp_size, temp_size);  // B
  glNormal3f(1, 0, 0);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -temp_size, -temp_size);  // F

  glNormal3f(1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(temp_size, -(temp_size - 0.1f), temp_size);  // D
  glNormal3f(1, 0, 0);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -temp_size, -temp_size);  // F
  glNormal3f(1, 0, 0);
  glTexCoord2f(0, 1);
  glVertex3f(temp_size, -(temp_size - 0.1f), -temp_size);  // H

  // LEFT
  glColor4ub(255, 255, 255, 255);
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, -(temp_size - 0.1f), -temp_size);  // G
  glNormal3f(-1, 0, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-temp_size, -temp_size, -temp_size);  // E
  glNormal3f(-1, 0, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-temp_size, -temp_size, temp_size);  // A

  glNormal3f(-1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, -(temp_size - 0.1f), -temp_size);  // G
  glNormal3f(-1, 0, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-temp_size, -temp_size, temp_size);  // A
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0, 1);
  glVertex3f(-temp_size, -(temp_size - 0.1f), temp_size);  // C

  // BOTTOM
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, -temp_size, temp_size);  // A
  glNormal3f(0, -1, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-temp_size, -temp_size, -temp_size);  // E
  glNormal3f(0, -1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -temp_size, -temp_size);  // F

  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, -temp_size, temp_size);  // A
  glNormal3f(0, -1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -temp_size, -temp_size);  // F
  glNormal3f(0, -1, 0);
  glTexCoord2f(0, 1);
  glVertex3f(temp_size, -temp_size, temp_size);  // B

  // TOP
  glColor4ub(255, 255, 255, 255);
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, -(temp_size - 0.1f), -temp_size);  // G
  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-temp_size, -(temp_size - 0.1f), temp_size);  // C
  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -(temp_size - 0.1f), temp_size);  // D

  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-temp_size, -(temp_size - 0.1f), -temp_size);  // G
  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(temp_size, -(temp_size - 0.1f), temp_size);  // D
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 1);
  glVertex3f(temp_size, -(temp_size - 0.1f), -temp_size);  // H
  glEnd();
}

void quick_primitives::double_plane() {
  glBegin(GL_TRIANGLES);
  // SIDE 1 FRONT
  glColor4ub(255, 255, 255, 255);
  glNormal3f(1, 0, 1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.25, 0.5, 0.25);  // C
  glNormal3f(1, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(-0.25, -0.5, 0.25);  // A
  glNormal3f(1, 0, 1);
  glTexCoord2f(1, 0);
  glVertex3f(0.25, -0.5, -0.25);  // B

  glNormal3f(1, 0, 1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.25, 0.5, 0.25);  // C
  glNormal3f(1, 0, 1);
  glTexCoord2f(1, 0);
  glVertex3f(0.25, -0.5, -0.25);  // B
  glNormal3f(1, 0, 1);
  glTexCoord2f(1, 1);
  glVertex3f(0.25, 0.5, -0.25);  // D

  // SIDE 1 BACK
  glColor4ub(255, 255, 255, 255);
  glNormal3f(-1, 0, -1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.25, 0.5, 0.25);  // C
  glNormal3f(-1, 0, -1);
  glTexCoord2f(1, 0);
  glVertex3f(0.25, -0.5, -0.25);  // B
  glNormal3f(-1, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(-0.25, -0.5, 0.25);  // A

  glNormal3f(-1, 0, -1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.25, 0.5, 0.25);  // C
  glNormal3f(-1, 0, -1);
  glTexCoord2f(1, 1);
  glVertex3f(0.25, 0.5, -0.25);  // D
  glNormal3f(-1, 0, -1);
  glTexCoord2f(1, 0);
  glVertex3f(0.25, -0.5, -0.25);  // B

  // SIDE 2 FRONT
  glColor4ub(255, 255, 255, 255);
  glNormal3f(-1, 0, 1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.25, 0.5, -0.25);  // C
  glNormal3f(-1, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(-0.25, -0.5, -0.25);  // A
  glNormal3f(-1, 0, 1);
  glTexCoord2f(1, 0);
  glVertex3f(0.25, -0.5, 0.25);  // B

  glNormal3f(-1, 0, 1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.25, 0.5, -0.25);  // C
  glNormal3f(-1, 0, 1);
  glTexCoord2f(1, 0);
  glVertex3f(0.25, -0.5, 0.25);  // B
  glNormal3f(-1, 0, 1);
  glTexCoord2f(1, 1);
  glVertex3f(0.25, 0.5, 0.25);  // D

  // SIDE 2 BACK
  glColor4ub(255, 255, 255, 255);
  glNormal3f(1, 0, -1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.25, 0.5, -0.25);  // C
  glNormal3f(1, 0, -1);
  glTexCoord2f(1, 0);
  glVertex3f(0.25, -0.5, 0.25);  // B
  glNormal3f(1, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(-0.25, -0.5, -0.25);  // A

  glNormal3f(1, 0, -1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.25, 0.5, -0.25);  // C
  glNormal3f(1, 0, -1);
  glTexCoord2f(1, 1);
  glVertex3f(0.25, 0.5, 0.25);  // D
  glNormal3f(1, 0, -1);
  glTexCoord2f(1, 0);
  glVertex3f(0.25, -0.5, 0.25);  // B
  glEnd();
}

void quick_primitives::double_plane_tall() {
  glBegin(GL_TRIANGLES);
  // SIDE 1 FRONT
  glColor4ub(255, 255, 255, 255);
  glNormal3f(1, 0, 1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.5, 2.5, 0.5);  // C
  glNormal3f(1, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(-0.5, -0.5, 0.5);  // A
  glNormal3f(1, 0, 1);
  glTexCoord2f(1, 0);
  glVertex3f(0.5, -0.5, -0.5);  // B

  glNormal3f(1, 0, 1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.5, 2.5, 0.5);  // C
  glNormal3f(1, 0, 1);
  glTexCoord2f(1, 0);
  glVertex3f(0.5, -0.5, -0.5);  // B
  glNormal3f(1, 0, 1);
  glTexCoord2f(1, 1);
  glVertex3f(0.5, 2.5, -0.5);  // D

  // SIDE 1 BACK
  glColor4ub(255, 255, 255, 255);
  glNormal3f(-1, 0, -1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.5, 2.5, 0.5);  // C
  glNormal3f(-1, 0, -1);
  glTexCoord2f(1, 0);
  glVertex3f(0.5, -0.5, -0.5);  // B
  glNormal3f(-1, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(-0.5, -0.5, 0.5);  // A

  glNormal3f(-1, 0, -1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.5, 2.5, 0.5);  // C
  glNormal3f(-1, 0, -1);
  glTexCoord2f(1, 1);
  glVertex3f(0.5, 2.5, -0.5);  // D
  glNormal3f(-1, 0, -1);
  glTexCoord2f(1, 0);
  glVertex3f(0.5, -0.5, -0.5);  // B

  // SIDE 2 FRONT
  glColor4ub(255, 255, 255, 255);
  glNormal3f(-1, 0, 1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.5, 2.5, -0.5);  // C
  glNormal3f(-1, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(-0.5, -0.5, -0.5);  // A
  glNormal3f(-1, 0, 1);
  glTexCoord2f(1, 0);
  glVertex3f(0.5, -0.5, 0.5);  // B

  glNormal3f(-1, 0, 1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.5, 2.5, -0.5);  // C
  glNormal3f(-1, 0, 1);
  glTexCoord2f(1, 0);
  glVertex3f(0.5, -0.5, 0.5);  // B
  glNormal3f(-1, 0, 1);
  glTexCoord2f(1, 1);
  glVertex3f(0.5, 2.5, 0.5);  // D

  // SIDE 2 BACK
  glColor4ub(255, 255, 255, 255);
  glNormal3f(1, 0, -1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.5, 2.5, -0.5);  // C
  glNormal3f(1, 0, -1);
  glTexCoord2f(1, 0);
  glVertex3f(0.5, -0.5, 0.5);  // B
  glNormal3f(1, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(-0.5, -0.5, -0.5);  // A

  glNormal3f(1, 0, -1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.5, 2.5, -0.5);  // C
  glNormal3f(1, 0, -1);
  glTexCoord2f(1, 1);
  glVertex3f(0.5, 2.5, 0.5);  // D
  glNormal3f(1, 0, -1);
  glTexCoord2f(1, 0);
  glVertex3f(0.5, -0.5, 0.5);  // B
  glEnd();
}

// Model
void quick_primitives::model_render(float primitive_size,
                                    const model& new_model) {
  // Push in case of scale
  glPushMatrix();
  // Scale
  glScaled(primitive_size, primitive_size, primitive_size);

  // Vertices
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, new_model.vertex_buffer);
  glVertexPointer(3, GL_FLOAT, sizeof(glm::vec3), nullptr);

  // Normals
  glEnableClientState(GL_NORMAL_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, new_model.normal_buffer);
  glNormalPointer(GL_FLOAT, sizeof(glm::vec3), nullptr);

  // Textures
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, new_model.texture_buffer);
  glTexCoordPointer(2, GL_FLOAT, sizeof(glm::vec2), nullptr);
  glBindTexture(GL_TEXTURE_2D, new_model.texture_id);

  // Send all info to ogl for drawing
  glDrawArrays(GL_TRIANGLES, 0, new_model.vertices.size());

  // Disable vbo arrays
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  // Pop in case of scale
  glPopMatrix();
}
