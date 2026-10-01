#pragma once

#include <GL/glew.h>
#include <memory>
#include <vector>

#include "./gui_element.h"

class Toolbar : public GuiElement
{
  public:
    Toolbar() = default;

    // Owns GL handles
    Toolbar(const Toolbar&)            = delete;
    Toolbar& operator=(const Toolbar&) = delete;

    ~Toolbar() override
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteTextures(1, &texture);
    }

    void render() const override
    {
        if (vao == 0)
        {
            createMesh();
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

  private:
    // Built on first render, so the GL context exists
    void createMesh() const
    {
        const float vertices[] = {
            // x, y, u, v
            100.0f, 100.0f, 0.0f, 0.0f, // bottom-left
            300.0f, 100.0f, 1.0f, 0.0f, // bottom-right
            300.0f, 200.0f, 1.0f, 1.0f, // top-right
            100.0f, 200.0f, 0.0f, 1.0f  // top-left
        };

        const unsigned int indices[] = {0, 1, 2, 2, 3, 0};

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        // Texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        // 1x1 white texture, so the gui shader draws the uColor tint
        const unsigned char white[] = {255, 255, 255, 255};
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    mutable GLuint vao{0};
    mutable GLuint vbo{0};
    mutable GLuint ebo{0};
    mutable GLuint texture{0};
};
