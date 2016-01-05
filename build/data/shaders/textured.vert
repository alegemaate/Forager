/* 
	Basic vertex shader 
	Allan Legemaate
	05/01/16
	Very basic vertex shader
*/
#version 120

varying vec3 lightDir,normal;

void main(){
	normal = normalize(gl_NormalMatrix * gl_Normal);
	lightDir = normalize(vec3(gl_LightSource[0].position));
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
} 