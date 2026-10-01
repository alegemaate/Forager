#version 300 es
precision highp float;

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec4 uColor;   // tint color (RGBA)

void main() {
    vec4 texColor = texture(uTexture, vUV);
    FragColor = texColor * uColor;

    // optional: discard transparent pixels
    // if (FragColor.a < 0.1)
    //     discard;
}