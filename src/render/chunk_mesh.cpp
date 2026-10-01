#include "./chunk_mesh.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../utils/loaders.h"

u32 ChunkMesh::atlas = 0;

ChunkMesh::~ChunkMesh()
{
    if (vao != 0)
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
}

void ChunkMesh::upload(const MeshData& data)
{
    opaqueIndices = data.opaqueIndices;
    waterIndices  = data.waterIndices;

    if (data.indices.empty())
    {
        return;
    }

    // Made on first upload, so empty chunks hold no GL objects
    if (vao == 0)
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(f32) * data.vertices.size()), data.vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(u32) * data.indices.size()),
                 data.indices.data(), GL_STATIC_DRAW);

    constexpr GLsizei stride = VERTEX_FLOATS * sizeof(f32);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    // Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    // AO attribute
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(f32)));
    glEnableVertexAttribArray(3);

    // Sky and block light attribute
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(f32)));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);
}

GLuint ChunkMesh::getAtlas()
{
    if (atlas == 0)
    {
        atlas = loaders::loadTexture("assets/images/textures/atlas.png");
    }
    return atlas;
}

void ChunkMesh::bindAtlas()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, getAtlas());
}

void ChunkMesh::releaseAtlas()
{
    glDeleteTextures(1, &atlas);
    atlas = 0;
}

void ChunkMesh::renderOpaque(GLint modelLocation, const glm::vec3& offset) const
{
    draw(modelLocation, offset, 0, opaqueIndices);
}

void ChunkMesh::renderWater(GLint modelLocation, const glm::vec3& offset) const
{
    draw(modelLocation, offset, opaqueIndices, waterIndices);
}

void ChunkMesh::draw(GLint modelLocation, const glm::vec3& offset, u32 first, u32 count) const
{
    if (count == 0)
    {
        return;
    }

    const glm::mat4 model = glm::translate(glm::mat4(1.0f), offset);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model[0][0]);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT,
                   reinterpret_cast<const void*>(static_cast<uintptr_t>(first) * sizeof(u32)));
}
