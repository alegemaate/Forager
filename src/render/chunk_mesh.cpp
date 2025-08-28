
#include "./chunk_mesh.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "../utils/loaders.h"
#include "../world/world.h"

u32 ChunkMesh::atlas = 0;

constexpr u32 ATLAS_WIDTH     = 8;
constexpr f32 ATLAS_WIDTH_INV = 1.0f / ATLAS_WIDTH;

// Construct
ChunkMesh::ChunkMesh()
{
    // Check atlas
    if (atlas == 0)
    {
        atlas = loaders::loadTexture("assets/images/textures/atlas.png");
    }

    // Make VAO, VBO, EBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

ChunkMesh::~ChunkMesh()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

// Fill array with given data
void ChunkMesh::fillFace(const FaceDefinition& face, const glm::vec3& base, const glm::vec3& worldPos, u32 atlasPos,
                         World& world)
{
    const u32 atlasX = atlasPos % ATLAS_WIDTH;
    const u32 atlasY = atlasPos / ATLAS_WIDTH;
    auto&     chunks = world.getChunks();

    // Reserve space
    const u32 baseIndex = static_cast<u32>(vertices.size() / 9);

    // Calculate neighbours
    for (u32 i = 0; i < face.neighbours.size(); i++)
    {
        neighbours[i] = chunks.isSolidAt(face.neighbours[i].x + worldPos.x, face.neighbours[i].y + worldPos.y,
                                         face.neighbours[i].z + worldPos.z);
    }

    // Push back vertices
    for (u32 i = 0; i < 4; i++)
    {
        // Sample neighbors adjacent to this face’s outside cell
        const bool solidCorner = neighbours[(2 * i) % 8];
        const bool solidSideA  = neighbours[(2 * i + 1) % 8];
        const bool solidSideB  = neighbours[(2 * i + 7) % 8];

        // "Hard corner" rule: if both sides are filled, corner doesn't matter
        const int occ = (solidSideA && solidSideB) ? 3 : (int)solidSideA + (int)solidSideB + (int)solidCorner;

        // Map 0..3 -> 1.0, 0.66, 0.33, 0.0 (tune if you want it less strong)
        const float ao = (3.0f - occ) / 3.0f;

        // Write vertex: position, normal, uv (atlas), AO
        const glm::vec3 pos = face.vertices[i] + base;

        vertices.push_back(pos.x);
        vertices.push_back(pos.y);
        vertices.push_back(pos.z);

        vertices.push_back(face.normal.x);
        vertices.push_back(face.normal.y);
        vertices.push_back(face.normal.z);

        vertices.push_back((faceUVs[i].x + atlasX) * ATLAS_WIDTH_INV);
        vertices.push_back((faceUVs[i].y + atlasY) * ATLAS_WIDTH_INV);

        vertices.push_back(ao);
    }

    // two tris
    static const u32 quadIdx[6] = {0, 1, 2, 0, 2, 3};
    for (int i = 0; i < 6; i++)
    {
        indices.push_back(baseIndex + quadIdx[i]);
    }
}

// Tessellate chunk
void ChunkMesh::tessellate(World& world, glm::vec3 position, Block (&blk)[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH])
{
    auto& chunks = world.getChunks();

    for (u32 i = 0; i < CHUNK_WIDTH; i++)
    {
        for (u32 t = 0; t < CHUNK_HEIGHT; t++)
        {
            for (u32 k = 0; k < CHUNK_LENGTH; k++)
            {
                auto* parent = blk[i][t][k].getTile();
                auto  type   = parent->getType();

                // Empty block?
                if (type == BlockID::Air)
                {
                    continue;
                }

                const auto&     atlasIds = parent->getAtlasIds();
                const glm::vec3 base     = glm::vec3(i, t, k);
                const glm::vec3 wPos     = glm::vec3(position) + base;

                VoxelNeighbours neighbours{};
                neighbours.top    = chunks.isSolidAt(wPos.x, wPos.y + 1, wPos.z);
                neighbours.bottom = chunks.isSolidAt(wPos.x, wPos.y - 1, wPos.z);
                neighbours.left   = chunks.isSolidAt(wPos.x - 1, wPos.y, wPos.z);
                neighbours.right  = chunks.isSolidAt(wPos.x + 1, wPos.y, wPos.z);
                neighbours.front  = chunks.isSolidAt(wPos.x, wPos.y, wPos.z + 1);
                neighbours.back   = chunks.isSolidAt(wPos.x, wPos.y, wPos.z - 1);

                if (!neighbours.top)
                {
                    fillFace(topFace, base, wPos, atlasIds.top, world);
                }
                if (!neighbours.bottom)
                {
                    fillFace(bottomFace, base, wPos, atlasIds.bottom, world);
                }
                if (!neighbours.left)
                {
                    fillFace(leftFace, base, wPos, atlasIds.left, world);
                }
                if (!neighbours.right)
                {
                    fillFace(rightFace, base, wPos, atlasIds.right, world);
                }
                if (!neighbours.front)
                {
                    fillFace(frontFace, base, wPos, atlasIds.front, world);
                }
                if (!neighbours.back)
                {
                    fillFace(backFace, base, wPos, atlasIds.back, world);
                }
            }
        }
    }

    if (vertices.empty() || indices.empty())
    {
        return;
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), nullptr);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    // Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    // AO coord attribute
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(8 * sizeof(f32)));
    glEnableVertexAttribArray(3);

    // Clear mesh
    numIndices = indices.size();
    vertices.clear();
    indices.clear();
}

void ChunkMesh::render(World& world, u32 offsetX, u32 offsetY, u32 offsetZ)
{
    auto& defaultShader = world.getGpuProgramManager().getShader("default");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas);

    glm::mat4 model = glm::translate(glm::mat4(1.0f),
                                     glm::vec3(offsetX * CHUNK_WIDTH, offsetY * CHUNK_HEIGHT, offsetZ * CHUNK_LENGTH));

    defaultShader.setMat4("model", model);

    // Render
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
