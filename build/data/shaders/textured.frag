/* 
	Basic fragment shader 
	Allan Legemaate
	05/01/16
	Very basic fragment shader, uses material and 
	applies lighting
*/
#version 120

varying vec3 lightDir,normal;
uniform sampler2D tex,l3d;

void main()
{
	// Variables
	vec3 texture_color, material_color, light_color;
	vec4 texel;
	float intensity, texture_alpha, material_alpha, light_alpha;
	
	
	// Get light intensity
	intensity = max(dot(lightDir,normalize(normal)),0.0);
	
	// Fetch material color and alpha 
	material_color = intensity * (gl_FrontMaterial.diffuse).rgb + 
								  gl_FrontMaterial.ambient.rgb;
	material_alpha = gl_FrontMaterial.diffuse.a;

	// Light info
	light_color = gl_LightSource[0].ambient.rgb + (intensity * gl_LightSource[0].diffuse.rgb);
	light_alpha = gl_LightSource[0].diffuse.a;
	
	// Get texture
	texel = texture2D(tex,gl_TexCoord[0].st) + 
			texture2D(l3d,gl_TexCoord[0].st);
	
	// Fetch texture color and alpha 
	texture_color = texel.rgb;
	texture_alpha = texel.a;
	
	// Stick em all together
	gl_FragColor = vec4( texture_color * material_color * light_color, texture_alpha * material_alpha * light_alpha);
}  
