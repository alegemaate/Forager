#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aUV;
layout (location=3) in float aAO;

out VS_OUT {
  vec2 uv;
  float ao;
  flat vec3 normal;   // flat for voxels
  float viewZ;        // for fog
} v;

uniform mat4 model, view, projection;

void main() {
  v.uv    = aUV;
  v.ao    = aAO;
  v.normal = normalize(mat3(transpose(inverse(model))) * aNormal);

  vec4 posVS = view * model * vec4(aPos, 1.0);
  v.viewZ = -posVS.z;
  gl_Position = projection * posVS;
}