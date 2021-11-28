#include "TileType.h"

#include "../constants/globals.h"
#include "../constants/ids.h"
#include "../utils/loaders.h"
#include "../utils/quick-primatives.h"
#include "MaterialManager.h"

// Init tile
TileType::TileType(const std::string& name,
                   unsigned char type,
                   ModelType model,
                   unsigned char attribute,
                   char randomness)
    : name(name),
      type(type),
      attribute(attribute),
      model(model),
      randomness(randomness) {}

// Set images from file
void TileType::setImages(const std::string& path1, const std::string& path2) {
  // Load image 1
  if (path1 != "NULL") {
    image_reference_number[0] = loaders::loadTexture(path1);
  }

  // Load image 2
  if (path2 != "NULL") {
    image_reference_number[1] = loaders::loadTexture(path2);
  } else {
    image_reference_number[1] = image_reference_number[0];
  }

  // No blurry textures!
  glBindTexture(GL_TEXTURE_2D, image_reference_number[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, image_reference_number[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// Draw tile
void TileType::draw(float x, float y, float z, bool newTick) {
  if (model == ModelType::MODEL_NONE) {
    return;
  }

  // Shader
  if (type == TILE_WATER) {
    waterShader->activate();
    changeMaterial(MATERIAL_WATER);
  }

  glBindTexture(GL_TEXTURE_2D, image_reference_number[newTick]);

  // Go into model view matrix
  glPushMatrix();

  // Translate in
  glTranslatef(x, y, z);

  if (model == ModelType::MODEL_CUBE || key[KEY_T]) {
    quick_primitives::cube(1.0f, 0.75f);
  }
  // Small Cube
  else if (model == ModelType::MODEL_CUBE_SMALL) {
    quick_primitives::model_render(0.25f, quick_primitives::test_model);
  }
  // Flat Cube
  else if (model == ModelType::MODEL_FLAT) {
    quick_primitives::cube_flat(1.0f);
  }
  // X like planes
  else if (model == ModelType::MODEL_DOUBLE_PLANE) {
    quick_primitives::double_plane();
  }
  // X like planes tall
  else if (model == ModelType::MODEL_DOUBLE_PLANE_TALL) {
    quick_primitives::double_plane_tall();
  }

  // Shader back
  if (currentMaterial != MATERIAL_DEFAULT) {
    changeMaterial(MATERIAL_DEFAULT);
    defaultShader->activate();
  }

  // Back
  glPopMatrix();
}
