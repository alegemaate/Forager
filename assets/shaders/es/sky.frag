#version 300 es
precision mediump float;

out vec4 FragColor;
in vec3 TexCoords;

uniform samplerCube skybox;
uniform vec3 ambient;

void main()
{
    FragColor = texture(skybox, TexCoords) * vec4(ambient, 1.0);
}
