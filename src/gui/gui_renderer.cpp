#include "./gui_renderer.h"

#include <asw/asw.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../block/block_registry.h"
#include "../render/chunk_mesh.h"
#include "./font_data.h"

namespace
{
// Floats per vertex: position 2, uv 2, colour 4
constexpr u32 GUI_VERTEX_FLOATS = 8;

// Font texture: one cell per glyph, with a pixel of space after each glyph
constexpr int CELL_WIDTH   = font::GLYPH_WIDTH + 1;
constexpr int CELL_HEIGHT  = font::GLYPH_HEIGHT + 1;
constexpr int FONT_COLUMNS = 16;
constexpr int FONT_ROWS    = font::CHAR_COUNT / FONT_COLUMNS;
constexpr int FONT_WIDTH   = FONT_COLUMNS * CELL_WIDTH;
constexpr int FONT_HEIGHT  = FONT_ROWS * CELL_HEIGHT;

constexpr u32 ATLAS_TILES = 8;

int glyphIndex(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        c = static_cast<char>(c - 'a' + 'A');
    }

    const int index = c - font::FIRST_CHAR;
    return (index >= 0 && index < font::CHAR_COUNT) ? index : '?' - font::FIRST_CHAR;
}

GLuint createTexture(int width, int height, const std::vector<unsigned char>& pixels)
{
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}
} // namespace

GuiRenderer::~GuiRenderer()
{
    if (vao != 0)
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteTextures(1, &whiteTexture);
        glDeleteTextures(1, &fontTexture);
    }
}

void GuiRenderer::init()
{
    shader.initProgramFromFiles({"gui.vert", "gui.frag"});

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    constexpr GLsizei stride = GUI_VERTEX_FLOATS * sizeof(f32);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(4 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // 1x1 white texture, so rectangles draw their vertex colour
    whiteTexture = createTexture(1, 1, {255, 255, 255, 255});

    // Font glyphs, white on clear
    std::vector<unsigned char> pixels(static_cast<size_t>(FONT_WIDTH) * FONT_HEIGHT * 4, 0);
    for (int glyph = 0; glyph < font::CHAR_COUNT; glyph++)
    {
        const int cellX = (glyph % FONT_COLUMNS) * CELL_WIDTH;
        const int cellY = (glyph / FONT_COLUMNS) * CELL_HEIGHT;

        for (int row = 0; row < font::GLYPH_HEIGHT; row++)
        {
            for (int col = 0; col < font::GLYPH_WIDTH; col++)
            {
                if ((font::GLYPHS[glyph][row] >> (font::GLYPH_WIDTH - 1 - col)) & 1U)
                {
                    const size_t i = ((static_cast<size_t>(cellY + row) * FONT_WIDTH) + cellX + col) * 4;
                    pixels[i]      = 255;
                    pixels[i + 1]  = 255;
                    pixels[i + 2]  = 255;
                    pixels[i + 3]  = 255;
                }
            }
        }
    }
    fontTexture = createTexture(FONT_WIDTH, FONT_HEIGHT, pixels);
}

void GuiRenderer::begin()
{
    const auto windowSize = asw::display::get_size();
    size                  = glm::vec2(static_cast<f32>(windowSize.x), static_cast<f32>(windowSize.y));

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.activate();
    shader.setMat4("projection", glm::ortho(0.0f, size.x, size.y, 0.0f, -1.0f, 1.0f));
    shader.setInt("uTexture", 0);
    glActiveTexture(GL_TEXTURE0);

    currentTexture = 0;
    useTexture(whiteTexture);
}

void GuiRenderer::end()
{
    flush();

    shader.deactivate();
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void GuiRenderer::useTexture(GLuint texture)
{
    if (texture == currentTexture)
    {
        return;
    }

    flush();
    currentTexture = texture;
    glBindTexture(GL_TEXTURE_2D, texture);
}

void GuiRenderer::flush()
{
    if (vertices.empty())
    {
        return;
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(f32)), vertices.data(),
                 GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / GUI_VERTEX_FLOATS));
    glBindVertexArray(0);

    vertices.clear();
}

void GuiRenderer::quad(f32 x, f32 y, f32 w, f32 h, glm::vec2 uv0, glm::vec2 uv1, const glm::vec4& color)
{
    const f32 corners[6][4] = {
        {x, y, uv0.x, uv0.y}, {x + w, y, uv1.x, uv0.y},     {x + w, y + h, uv1.x, uv1.y},
        {x, y, uv0.x, uv0.y}, {x + w, y + h, uv1.x, uv1.y}, {x, y + h, uv0.x, uv1.y},
    };

    for (const auto& c : corners)
    {
        vertices.insert(vertices.end(), {c[0], c[1], c[2], c[3], color.r, color.g, color.b, color.a});
    }
}

void GuiRenderer::rect(f32 x, f32 y, f32 w, f32 h, const glm::vec4& color)
{
    useTexture(whiteTexture);
    quad(x, y, w, h, {0.0f, 0.0f}, {1.0f, 1.0f}, color);
}

void GuiRenderer::rectOutline(f32 x, f32 y, f32 w, f32 h, f32 thickness, const glm::vec4& color)
{
    rect(x, y, w, thickness, color);
    rect(x, y + h - thickness, w, thickness, color);
    rect(x, y + thickness, thickness, h - (2 * thickness), color);
    rect(x + w - thickness, y + thickness, thickness, h - (2 * thickness), color);
}

void GuiRenderer::blockIcon(BlockID id, f32 x, f32 y, f32 iconSize, const glm::vec4& tint)
{
    const u32       tile = BlockRegistry::get(id).getAtlasIds().front;
    const f32       step = 1.0f / ATLAS_TILES;
    const glm::vec2 uv0(static_cast<f32>(tile % ATLAS_TILES) * step, static_cast<f32>(tile / ATLAS_TILES) * step);

    useTexture(ChunkMesh::getAtlas());
    quad(x, y, iconSize, iconSize, uv0, uv0 + glm::vec2(step), tint);
}

f32 GuiRenderer::textWidth(std::string_view str, f32 scale)
{
    if (str.empty())
    {
        return 0.0f;
    }
    return ((static_cast<f32>(str.size()) * CELL_WIDTH) - 1.0f) * scale;
}

f32 GuiRenderer::textHeight(f32 scale)
{
    return font::GLYPH_HEIGHT * scale;
}

void GuiRenderer::text(std::string_view str, f32 x, f32 y, f32 scale, const glm::vec4& color, TextAlign align)
{
    if (align == TextAlign::Center)
    {
        x -= textWidth(str, scale) / 2.0f;
    }
    else if (align == TextAlign::Right)
    {
        x -= textWidth(str, scale);
    }

    // Snap to whole points, so pixels stay square
    x = std::round(x);
    y = std::round(y);

    useTexture(fontTexture);

    const glm::vec2 cellUV(1.0f / FONT_COLUMNS, 1.0f / FONT_ROWS);
    const glm::vec2 glyphUV(static_cast<f32>(font::GLYPH_WIDTH) / FONT_WIDTH,
                            static_cast<f32>(font::GLYPH_HEIGHT) / FONT_HEIGHT);

    for (const char c : str)
    {
        if (c != ' ')
        {
            const int       glyph = glyphIndex(c);
            const glm::vec2 uv0(static_cast<f32>(glyph % FONT_COLUMNS) * cellUV.x,
                                static_cast<f32>(glyph / FONT_COLUMNS) * cellUV.y);
            quad(x, y, font::GLYPH_WIDTH * scale, font::GLYPH_HEIGHT * scale, uv0, uv0 + glyphUV, color);
        }
        x += CELL_WIDTH * scale;
    }
}

void GuiRenderer::textShadow(std::string_view str, f32 x, f32 y, f32 scale, const glm::vec4& color, TextAlign align)
{
    text(str, x + scale, y + scale, scale, glm::vec4(0.0f, 0.0f, 0.0f, color.a * 0.6f), align);
    text(str, x, y, scale, color, align);
}
