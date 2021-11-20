#include "tile_type.h"

// Init tile
tile_type::tile_type(std::string newName,
                     unsigned char newType,
                     BITMAP* defaultImage,
                     std::string newModel,
                     unsigned char newAttribute,
                     char newRandomness) {
  // Set init variables
  type = newType;
  name = newName;
  model = newModel;

  image[0] = defaultImage;
  image[1] = defaultImage;

  image_reference_number[0] = 0;
  image_reference_number[1] = 0;

  randomness = newRandomness;

  attribute = newAttribute;
}

// Destroy tile
tile_type::~tile_type() {
  // dtor
}

// Set images from file
void tile_type::setImages(std::string path1, std::string path2) {
  // Load image 1
  if (path1 != "NULL") {
    image[0] = load_bitmap(path1.c_str(), NULL);
    image_reference_number[0] = allegro_gl_make_texture_ex(
        AGL_TEXTURE_HAS_ALPHA | AGL_TEXTURE_FLIP, image[0], GL_RGBA);
    texture_reference[type] = image_reference_number[0];
  }

  // Load image 2
  if (path2 != "NULL") {
    image[1] = load_bitmap(path2.c_str(), NULL);
    image_reference_number[1] = allegro_gl_make_texture_ex(
        AGL_TEXTURE_HAS_ALPHA | AGL_TEXTURE_FLIP, image[1], GL_RGBA);
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
void tile_type::draw(double x, double y, double z, bool newTick) {
  if (true) {
    // Make sure there is a model
    if (model != "MODEL_NONE") {
      // Shader
      if (type == TILE_WATER) {
        glUseProgram(waterShader);
        changeMaterial(MATERIAL_WATER);
      }

      glBindTexture(GL_TEXTURE_2D, image_reference_number[newTick]);

      // Go into modelview matrix
      glPushMatrix();

      // Translate in
      glTranslatef(x, y, z);

      if (model == "MODEL_CUBE" || key[KEY_T]) {
        quick_primatives::cube(1.0f, 0.75f);
      }
      // Small Cube
      else if (model == "MODEL_CUBE_SMALL") {
        quick_primatives::model_render(0.25f, quick_primatives::test_model);
      }
      // Flat Cube
      else if (model == "MODEL_FLAT") {
        quick_primatives::cube_flat(1.0f);
      }
      // X like planes
      else if (model == "MODEL_DOUBLE_PLANE") {
        quick_primatives::double_plane(1.0f);
      }
      // X like planes tall
      else if (model == "MODEL_DOUBLE_PLANE_TALL") {
        quick_primatives::double_plane_tall(1.0f);
      }

      // Shader back
      if (currentMaterial != MATERIAL_DEFAULT) {
        changeMaterial(MATERIAL_DEFAULT);
        glUseProgram(defaultShader);
      }

      // Back
      glPopMatrix();
    }
  }
}
