#version 300 es
precision highp float;

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aUV;
layout (location=3) in float aAO;
layout (location=4) in vec2 aLight; // sky, block

out vec2 vUV;
out float vAO;
out vec2 vLight;
flat out vec3 vNormal; // flat for voxels
out float vDist;       // for fog

uniform mat4 model, view, projection;
uniform float uTime;
uniform int uWater;

void main() {
  vUV = aUV;
  vAO = aAO;
  vLight = aLight;
  vNormal = aNormal;

  vec4 world = model * vec4(aPos, 1.0);

  // Gentle waves on water
  if (uWater == 1) {
    world.y += sin(uTime * 1.5 + world.x * 0.7 + world.z * 0.5) * 0.04 - 0.04;
  }

  vec4 posVS = view * world;
  vDist = length(posVS.xyz);
  gl_Position = projection * posVS;
}
