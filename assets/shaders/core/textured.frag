#version 330 core

out vec4 FragColor;

in vec2 vUV;
in float vAO;
in vec2 vLight;
flat in vec3 vNormal;
in float vDist;

struct Light {
  vec3 direction;   // direction the light travels
  vec3 color;       // diffuse color
  vec3 ambient;     // ambient color
};

uniform Light light;
uniform sampler2D atlas;
uniform int uWater;
uniform int uUnderwater;
uniform vec3 uFogColor;
uniform float uFogFar;

const vec3 TORCH_COLOR = vec3(1.0, 0.82, 0.6);
const vec3 UNDERWATER_COLOR = vec3(0.05, 0.18, 0.35);

// Light levels fall off like 0.8^steps, so light fades quickly near its edge
float curve(float level) {
  return pow(0.8, (1.0 - level) * 15.0);
}

void main() {
  vec4 texel = texture(atlas, vUV);

  vec3 n = normalize(vNormal);
  vec3 l = normalize(-light.direction);
  float ndotl = max(dot(n, l), 0.0);

  float ao = mix(0.55, 1.0, clamp(vAO, 0.0, 1.0));
  float sky = curve(vLight.x);
  float torch = vLight.y > 0.0 ? curve(vLight.y) : 0.0;

  // Sun and sky light reach open places, torches light the rest
  vec3 sunLight = (light.ambient * ao + light.color * ndotl) * sky;
  vec3 torchLight = TORCH_COLOR * torch * 1.2 * ao;
  vec3 lighting = max(sunLight, torchLight) + vec3(0.02);

  vec3 color = texel.rgb * lighting;

  // Fog thickens toward the edge of the view distance
  float fog = smoothstep(uFogFar * 0.55, uFogFar, vDist);
  vec3 fogColor = uFogColor;

  if (uUnderwater == 1) {
    fog = 1.0 - exp(-0.09 * vDist);
    fogColor = UNDERWATER_COLOR * max(light.ambient.r, 0.3);
  }

  color = mix(color, fogColor, clamp(fog, 0.0, 1.0));

  float alpha = uWater == 1 ? 0.72 : 1.0;
  FragColor = vec4(color, alpha);
}
