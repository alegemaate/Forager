#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in float aAO;

out vec2 vUV;
out float vAO;
out vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vUV = aUV;
    vAO = aAO;
    vNormal = normalize(mat3(transpose(inverse(model))) * aNormal);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
