#version 330 core

in vec4 fColor;
in vec4 fEye;
in vec4 fNormal;
in vec4 fShadow;
in vec4 fWorld;
in vec2 UVcoords;

out vec4 FragColor;
layout (location = 0) out vec4 color;

uniform vec3 lightPos1;
uniform vec3 lightColor1;

//uniform vec3 lightPos2;
//uniform vec3 lightColor2;

uniform sampler2D myTextureSampler;
uniform sampler2DShadow depth_texture;
uniform bool fRender;

uniform float kD; // diffuse
uniform float kA; // ambient
uniform float kS; // specular

void main()
{
	if(fRender){
		FragColor = vec4(1.0);
	}else{
		vec4 texel = texture2D(myTextureSampler,UVcoords);
		
		FragColor = kA * texel;	//ambient color
		float diff, spec;
		
		float f = textureProj(depth_texture, fShadow);
		
		//light source 1
		diff = max(dot(fNormal, normalize(vec4(lightPos1, 1.) - fEye)), 0.0);
		spec = max(dot(normalize(-fEye), reflect(-normalize(vec4(lightPos1, 1.) - fEye), fNormal)), 0.0);
	
		FragColor += f * kD * diff * (0.3 * vec4(lightColor1, 1.) + 0.7 * texel);
		FragColor += f * kS * pow(spec, 2) * (0.5 * vec4(lightColor1, 1.) + 0.5 * vec4(1., 1., 1., 1.));
		
		//light source 2
		/*diff = max(dot(fNormal, normalize(vec4(lightPos2, 1.) - fEye)), 0.0);
		spec = max(dot(normalize(-fEye), reflect(-normalize(vec4(lightPos2, 1.) - fEye), fNormal)), 0.0);
	
		FragColor += kD * diff * (0.3 * vec4(lightColor2, 1.) + 0.7  * texel);
		FragColor += kS * pow(spec, 2) * (0.5 * vec4(lightColor2, 1.) + 0.5 * vec4(1., 1., 1., 1.));*/
	}
}
