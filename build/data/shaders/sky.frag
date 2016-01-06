/* 
	Skybox Day Night Shader
	Allan Legemaate
	05/01/16
	Changes with TIMER
*/
#version 120

uniform sampler2D tex;
uniform sampler2D sampler;

uniform float timer;

varying vec2 fragment_uv;

void main() {
	// Variables
	vec3 texture_color, bias_color;
	vec4 texture, biasTexture;
	float texture_alpha, bias_alpha;

    vec2 uv = vec2(timer, fragment_uv.t);
    
	// Get texture
	texture = texture2D( tex, gl_TexCoord[0].st);
	biasTexture = texture2D( sampler, uv);

	// Fetch texture color and alpha 
	texture_color = texture.rgb;
	texture_alpha = texture.a;

	bias_color = biasTexture.rgb;
	bias_alpha = biasTexture.a;
	
	// Stick em all together
	gl_FragColor = vec4( texture_color * bias_color, texture_alpha * bias_alpha);
}
