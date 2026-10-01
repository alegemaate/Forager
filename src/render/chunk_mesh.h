#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../core/Types.h"
#include "./mesher.h"

using namespace core;

/// @brief GPU copy of a chunk's geometry. Built by the mesher, uploaded on the GL thread.
class ChunkMesh
{
  public:
    ChunkMesh() = default;
    ~ChunkMesh();

    // Owns GL handles
    ChunkMesh(const ChunkMesh&)            = delete;
    ChunkMesh& operator=(const ChunkMesh&) = delete;
    ChunkMesh(ChunkMesh&&)                 = delete;
    ChunkMesh& operator=(ChunkMesh&&)      = delete;

    /// @brief Replace the geometry
    void upload(const MeshData& data);

    /// @brief Draw solid geometry. The caller activates the shader and binds the atlas.
    void renderOpaque(GLint modelLocation, const glm::vec3& offset) const;

    /// @brief Draw water. The caller sets up blending.
    void renderWater(GLint modelLocation, const glm::vec3& offset) const;

    bool hasWater() const
    {
        return waterIndices > 0;
    }

    bool empty() const
    {
        return opaqueIndices == 0 && waterIndices == 0;
    }

    /// @brief Bind the shared texture atlas to texture unit 0
    static void bindAtlas();

    /// @brief Shared texture atlas, loaded on first use
    static GLuint getAtlas();

    /// @brief Free the shared texture atlas. Call before the GL context goes away.
    static void releaseAtlas();

  private:
    void draw(GLint modelLocation, const glm::vec3& offset, u32 first, u32 count) const;

    u32 vao{0};
    u32 vbo{0};
    u32 ebo{0};

    u32 opaqueIndices{0};
    u32 waterIndices{0};

    static u32 atlas; // Texture atlas
};
