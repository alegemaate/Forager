/* 
	Basic fragment shader 
	Allan Legemaate
	05/01/16
	Very basic fragment shader, uses material and 
	applies lighting
*/
#version 120

varying vec3 lightDir,normal;
uniform sampler2D texture1,texture2;

void main()
{
	// Variables
	vec3 texture_color, material_color, light_color;
	vec4 texture_info;
	float intensity, texture_alpha, material_alpha, light_alpha;
	
	
	// Get light intensity
	intensity = max(dot(lightDir,normalize(normal)),0.0);
	
	// Fetch material color and alpha 
	material_color =  max( intensity * gl_FrontMaterial.diffuse.rgb, gl_FrontMaterial.ambient.rgb);
	material_alpha = gl_FrontMaterial.diffuse.a;

	// Light info
	light_color = max( intensity * gl_LightSource[0].diffuse.rgb, gl_LightSource[0].ambient.rgb);
	light_alpha = gl_LightSource[0].diffuse.a;
	
	// Get texture info
	texture_info = texture2D(texture1,gl_TexCoord[0].st) + 
			texture2D(texture2,gl_TexCoord[0].st);
	
	// Fetch texture color and alpha 
	texture_color = texture_info.rgb;
	texture_alpha = texture_info.a;
	
	// Stick em all together
	gl_FragColor = vec4( texture_color * material_color * light_color, texture_alpha * material_alpha * light_alpha);
}  