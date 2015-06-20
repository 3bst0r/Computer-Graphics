#version 330 core

in vec4 fColor;
in vec4 fEye;
in vec4 fNormal;
in vec2 UVcoords;

out vec4 FragColor;

uniform vec3 lightPos1;
uniform vec3 lightColor1;

uniform vec3 lightPos2;
uniform vec3 lightColor2;

uniform sampler2D myTextureSampler;

uniform float kD; // diffuse
uniform float kA; // ambient
uniform float kS; // specular


void main()
{
    FragColor = kA *  texture2D(myTextureSampler, UVcoords);	//ambient color
	float diff, spec;
	
	//light source 1
	diff = max(dot(fNormal, normalize(vec4(lightPos1, 1.) - fEye)), 0.0);
	spec = max(dot(normalize(-fEye), reflect(-normalize(vec4(lightPos1, 1.) - fEye), fNormal)), 0.0);

	FragColor += kD * diff * (0.3 * vec4(lightColor1, 1.) + 0.7 * fColor);
	FragColor += kS * pow(spec, 2) * (0.5 * vec4(lightColor1, 1.) + 0.5 * vec4(1., 1., 1., 1.));
	
	//light source 2
	diff = max(dot(fNormal, normalize(vec4(lightPos2, 1.) - fEye)), 0.0);
	spec = max(dot(normalize(-fEye), reflect(-normalize(vec4(lightPos2, 1.) - fEye), fNormal)), 0.0);

	FragColor += kD * diff * (0.3 * vec4(lightColor2, 1.) + 0.7 * fColor);
	FragColor += kS * pow(spec, 2) * (0.5 * vec4(lightColor2, 1.) + 0.5 * vec4(1., 1., 1., 1.));
}
