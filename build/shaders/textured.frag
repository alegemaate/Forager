/*
	Basic fragment shader
	Allan Legemaate
	05/01/16
	Very basic fragment shader, uses material and
	applies lighting
*/
#version 120

varying vec3 lightDir, normal;
uniform sampler2D texture1, texture2;

void main() {
    // Get light intensity
    float intensity = max(dot(lightDir, normalize(normal)), 0.0);

    // Fetch material color and alpha
    vec3 material_color = max(intensity * gl_FrontMaterial.diffuse.rgb, gl_FrontMaterial.ambient.rgb);
    float material_alpha = gl_FrontMaterial.diffuse.a;

    // Light info
    vec3 light_color = max(intensity * gl_LightSource[0].diffuse.rgb, gl_LightSource[0].ambient.rgb);
    float light_alpha = gl_LightSource[0].diffuse.a;

    // Get texture info
    vec4 texture_info = texture2D(texture1, gl_TexCoord[0].st) + texture2D(texture2, gl_TexCoord[0].st);

    // Fetch texture color and alpha
    vec3 texture_color = texture_info.rgb;
    float texture_alpha = texture_info.a;

    // Stick em all together
    gl_FragColor = vec4(texture_color * material_color * light_color, texture_alpha * material_alpha * light_alpha);
}