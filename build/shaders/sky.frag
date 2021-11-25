/* 
	Skybox Day Night Shader
	Allan Legemaate
	05/01/16
	Changes with TIMER
*/
#version 120

uniform sampler2D tex;
uniform sampler2D samp;

uniform float timer;

varying vec2 fragment_uv;

void main() {
    vec2 uv = vec2(timer, fragment_uv.t);

    // Get texture
    vec4 texture = texture2D(tex, gl_TexCoord[0].st);
    vec4 biasTexture = texture2D(samp, uv);

    // Fetch texture color and alpha
    vec3 texture_color = texture.rgb;
    float texture_alpha = texture.a;

    vec3 bias_color = biasTexture.rgb;
    float bias_alpha = biasTexture.a;

    // Stick em all together
    gl_FragColor = vec4(texture_color * bias_color, texture_alpha * bias_alpha);
}
