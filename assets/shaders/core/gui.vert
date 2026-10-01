#version 330 core

layout (location=0) in vec2 aPos;   // 2D position in screen space (x,y)
layout (location=1) in vec2 aUV;    // texture coordinates

out vec2 vUV;

uniform mat4 projection; // usually an orthographic matrix

void main() {
    vUV = aUV;
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
}
