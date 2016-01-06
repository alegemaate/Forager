/* 
	Skybox Day Night Shader
	Allan Legemaate
	05/01/16
	Changes with TIMER
*/
#version 120

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
}
