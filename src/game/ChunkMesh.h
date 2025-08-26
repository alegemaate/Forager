
#pragma once

#include <GL/glew.h>
#include <array>
#include <functional>
#include <vector>

#include "../core/Types.h"
#include "CubeFaces.h"
#include "Voxel.h"

using namespace core;

constexpr size_t CHUNK_WIDTH      = 16;
constexpr float  CHUNK_WIDTH_INV  = 1.0f / CHUNK_WIDTH;
constexpr size_t CHUNK_HEIGHT     = 128;
constexpr float  CHUNK_HEIGHT_INV = 1.0f / CHUNK_HEIGHT;
constexpr size_t CHUNK_LENGTH     = 16;
constexpr float  CHUNK_LENGTH_INV = 1.0f / CHUNK_LENGTH;

class World;

struct VoxelNeighbours
{
    bool top;
    bool bottom;
    bool left;
    bool right;
    bool front;
    bool back;
};

class ChunkMesh
{
  public:
    ChunkMesh();
    ~ChunkMesh();

    // Fill a given face
    void fillFace(const FaceDefinition& face, const glm::vec3& base, const glm::vec3& worldPos, GLuint atlasPos,
                  World& world);

    // Tessellate chunk
    void tessellate(World& world, glm::vec3 position, Voxel (&blk)[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]);

    // Render it all
    void render(World& world, u32 offsetX, u32 offsetY, u32 offsetZ);

  private:
    u32 vao{0};
    u32 vbo{0};
    u32 ebo{0};

    u32 numIndices{0};

    std::vector<f32> vertices;
    std::vector<u32> indices;

    static u32 atlas; // Texture atlas
};
