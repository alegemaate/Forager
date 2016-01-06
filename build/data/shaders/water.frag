/* 
	Water fragment shader 
	Allan Legemaate
	05/01/16
	This gets texture and light info
*/
#version 120

varying vec3 lightDir,normal;
uniform sampler2D tex,l3d;

void main()
{
	vec3 ct,cf;
	vec4 texel;
	float intensity,at,af;
	
	intensity = max(dot(lightDir,normalize(normal)),0.0);
	
	cf = intensity * (gl_FrontMaterial.diffuse).rgb + 
					  gl_FrontMaterial.ambient.rgb;
	af = gl_FrontMaterial.diffuse.a;
	
	texel = texture2D(tex,gl_TexCoord[0].st)+
		  texture2D(l3d,gl_TexCoord[0].st);
	ct = texel.rgb;
	at = texel.a;
	
	gl_FragColor = vec4(ct * cf, at * af);
}  