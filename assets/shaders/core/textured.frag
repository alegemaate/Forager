#version 330 core
out vec4 FragColor;

in VS_OUT {
  vec2 uv;
  float ao;
  flat vec3 normal;
  float viewZ;
} v;

struct Light {
  vec3 direction;   // world/eye-space, normalized
  vec3 color;       // diffuse color
  vec3 ambient;     // ambient color
};

uniform Light light;
uniform sampler2D atlas;

void main() {
  vec4 texel = texture(atlas, v.uv);

  vec3 n = normalize(v.normal);
  vec3 l = normalize(-light.direction);

  float ndotl = max(dot(n, l), 0.0);

  // Hemisphere fill (cheap skylight), optional:
  vec3 sky = vec3(0.65, 0.70, 0.90);
  vec3 ground = vec3(0.35, 0.30, 0.25);
  vec3 hemi = mix(ground, sky, n.y * 0.5 + 0.5) * 0.4;

  // Ambient modulated by AO (kept in a 0.75–1.0 range)
  vec3 ambient = light.ambient * mix(0.75, 1.0, clamp(v.ao, 0.0, 1.0));

  // Calculate lighting
  vec3 lighting = ambient + light.color * ndotl + hemi;
  vec3 colorLinear = texel.rgb * lighting;

  // Exponential fog by view distance
  float fogDensity = 0.01; 
  float fog = 1.0 - exp(-fogDensity * v.viewZ);
  colorLinear = mix(colorLinear, vec3(0, 0, 0), clamp(fog, 0.0, 1.0));

  FragColor = vec4(colorLinear, 1.0);
}