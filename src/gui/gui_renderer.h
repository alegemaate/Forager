/// @file gui_renderer.h
///
/// Batched 2D drawing for menus and the HUD: rectangles, block icons and pixel text. Positions are in window
/// points with the origin at the top left.
///
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string_view>
#include <vector>

#include "../block/block_type.h"
#include "../core/Types.h"
#include "../render/gpu_program.h"

using namespace core;

enum class TextAlign
{
    Left,
    Center,
    Right,
};

class GuiRenderer
{
  public:
    GuiRenderer() = default;
    ~GuiRenderer();

    // Owns GL handles
    GuiRenderer(const GuiRenderer&)            = delete;
    GuiRenderer& operator=(const GuiRenderer&) = delete;

    /// @brief Create GL objects. Call once the GL context exists.
    void init();

    /// @brief Start a frame of GUI drawing. Sets the GL state 2D drawing needs.
    void begin();

    /// @brief Draw everything queued and restore 3D state
    void end();

    /// @brief Window size in points
    glm::vec2 getSize() const
    {
        return size;
    }

    void rect(f32 x, f32 y, f32 w, f32 h, const glm::vec4& color);

    void rectOutline(f32 x, f32 y, f32 w, f32 h, f32 thickness, const glm::vec4& color);

    /// @brief Draw a block's front texture
    void blockIcon(BlockID id, f32 x, f32 y, f32 iconSize, const glm::vec4& tint = glm::vec4(1.0f));

    /// @brief Draw text. Scale is points per font pixel.
    void text(std::string_view str, f32 x, f32 y, f32 scale, const glm::vec4& color, TextAlign align = TextAlign::Left);

    /// @brief Draw text with a dark drop shadow, for text over the world
    void textShadow(std::string_view str, f32 x, f32 y, f32 scale, const glm::vec4& color,
                    TextAlign align = TextAlign::Left);

    static f32 textWidth(std::string_view str, f32 scale);

    static f32 textHeight(f32 scale);

  private:
    void quad(f32 x, f32 y, f32 w, f32 h, glm::vec2 uv0, glm::vec2 uv1, const glm::vec4& color);
    void useTexture(GLuint texture);
    void flush();

    GpuProgram shader;

    GLuint vao{0};
    GLuint vbo{0};
    GLuint whiteTexture{0};
    GLuint fontTexture{0};
    GLuint currentTexture{0};

    std::vector<f32> vertices;
    glm::vec2        size{0.0f};
};
