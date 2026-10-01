
#pragma once

#include <GL/glew.h>
#include <array>
#include <functional>
#include <vector>

#include "../block/block.h"
#include "../core/Types.h"
#include "./cube_faces.h"

using namespace core;

constexpr u32 CHUNK_WIDTH       = 16;
constexpr u32 CHUNK_HEIGHT      = 128;
constexpr u32 CHUNK_LENGTH      = 16;
constexpr u32 CHUNK_WIDTH_LOG2  = 4;
constexpr u32 CHUNK_LENGTH_LOG2 = 4;

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

    // Owns GL handles. Move takes them, so a vector<Chunk> reallocation cannot delete them twice
    ChunkMesh(const ChunkMesh&)            = delete;
    ChunkMesh& operator=(const ChunkMesh&) = delete;
    ChunkMesh(ChunkMesh&& other) noexcept;
    ChunkMesh& operator=(ChunkMesh&& other) noexcept;

    // Fill a given face
    void fillFace(const FaceDefinition& face, const glm::vec3& base, const glm::ivec3& worldPos, GLuint atlasPos,
                  World& world);

    // Tessellate chunk
    void tessellate(World& world, glm::ivec3 position, Block (&blk)[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]);

    // Render it all. The caller activates the shader and binds the atlas.
    void render(GLint modelLocation, const glm::vec3& offset) const;

    // True when there is nothing to draw
    bool empty() const
    {
        return numIndices == 0;
    }

    // Bind the shared texture atlas to texture unit 0
    static void bindAtlas();

  private:
    u32 vao{0};
    u32 vbo{0};
    u32 ebo{0};

    u32 numIndices{0};

    static u32 atlas; // Texture atlas
};
