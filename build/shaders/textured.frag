#version 330 core
out vec4 FragColor;

struct Light {
    vec3 direction;
    vec3 ambient;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform Light light;
uniform vec3 viewPos;
uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D texture1;

float near = 0.1;
float far  = 100.0;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;// back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}


void main() {
    vec3 lightColor = vec3(1.0f, 0.5f, 1.0f);

    // ambient
    vec3 ambient = light.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.ambient * diff;

    // depth
    float depth = 1.0f - (LinearizeDepth(gl_FragCoord.z) / far);


    // final color
    vec3 result = texture(texture1, TexCoord).rgb * vec3(depth) * (ambient + diffuse);
    FragColor = vec4(result, 1.0);
}

