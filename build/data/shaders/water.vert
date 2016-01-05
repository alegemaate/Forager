/* 
	Water vertex shader 
	Allan Legemaate
	05/01/16
	This changes the Y coordinate of a model by the bias
	and then makes it wave.
*/
#version 120

varying vec3 lightDir,normal;

uniform float waveTime;
uniform float waveWidth;
uniform float waveHeight;
 
void main(void)
{
	normal = normalize(gl_NormalMatrix * gl_Normal);
	lightDir = normalize(vec3(gl_LightSource[0].position));
	
	vec4 v = vec4(gl_Vertex);
	v.y = (sin(waveWidth * v.x + waveTime) * cos(waveWidth * v.z + waveTime) * waveHeight) + v.y/10;
 	gl_Position = gl_ModelViewProjectionMatrix * v;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
