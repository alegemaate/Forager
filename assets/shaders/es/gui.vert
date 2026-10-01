#version 300 es
precision highp float;

layout (location=0) in vec2 aPos;   // screen position, top left origin
layout (location=1) in vec2 aUV;    // texture coordinates
layout (location=2) in vec4 aColor; // tint

out vec2 vUV;
out vec4 vColor;

uniform mat4 projection; // orthographic

void main() {
    vUV = aUV;
    vColor = aColor;
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
}
