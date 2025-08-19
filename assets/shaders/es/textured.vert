#version 300 es
precision highp float;

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aUV;
layout (location=3) in float aAO;

out vec2 vUV;
out float vAO;
flat out vec3 vNormal;
out float vViewZ;

uniform mat4 model, view, projection;

void main() {
  vUV = aUV;
  vAO = aAO;
  vNormal = normalize(mat3(transpose(inverse(model))) * aNormal);

  vec4 posVS = view * model * vec4(aPos, 1.0);
  vViewZ = -posVS.z;
  gl_Position = projection * posVS;
}