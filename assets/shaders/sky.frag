/* 
	Skybox Day Night Shader
	Allan Legemaate
	05/01/16
	Changes with TIMER
*/
#version 120

uniform float timer;

void main() {
    vec4 biasTexture = texture2D(gl_TexCoord[1], timer);
    gl_FragColor = gl_TexCoord[0] * biasTexture;
}
