#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform vec3 ambient;

void main()
{
    FragColor = texture(skybox, TexCoords) * vec4(ambient.x, ambient.y, ambient.z, 1.0f);
}
