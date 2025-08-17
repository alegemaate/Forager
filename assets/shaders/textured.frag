#version 330 core
out vec4 FragColor;

struct Light {
    vec3 direction;
    vec3 ambient;
};

in vec2 vUV;
in float vAO;
in vec3 vNormal;

uniform Light light;
uniform vec3 viewPos;
uniform vec3 cameraPos;
uniform sampler2D atlas;

float near = 0.1;
float far  = 500.0;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;// back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    // Sample the texture atlas
    vec4 texel = texture(atlas, vUV);

    // Diffuse lighting
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = light.ambient * max(dot(vNormal, lightDir), 0.0);
    vec3 light = light.ambient * (vAO * 0.25 + 0.75) + diffuse;

    // depth
    vec3 depth = vec3(1.0f - (LinearizeDepth(gl_FragCoord.z) / far));

    // final color
    vec3 rgb = texel.rgb * depth * light; // Adjusted AO contribution
    FragColor = vec4(rgb, 1.0);
}
