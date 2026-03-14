#pragma once

#include <GL/gl.h>
#include <memory>
#include <vector>

#include "./gui_element.h"

class Toolbar : public GuiElement
{
  public:
    Toolbar() = default;

    void render() const override
    {
        unsigned int vao, vbo, ebo;

        float vertices[] = {
            // x, y
            100.0f, 100.0f, // bottom-left
            300.0f, 100.0f, // bottom-right
            300.0f, 200.0f, // top-right
            100.0f, 200.0f  // top-left
        };

        unsigned int indices[] = {0, 1, 2, 2, 3, 0};

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
};