#version 330 core

in vec4 fColor;
in vec4 fEye;
in vec4 fNormal;

out vec4 FragColor;

uniform vec3 lightPos1;
uniform vec3 lightColor1;

uniform vec3 lightPos2;
uniform vec3 lightColor2;

uniform float kD; // diffuse
uniform float kA; // ambient
uniform float kS; // specular


void main()
{
    FragColor = kA * fColor;	//ambient color
	float diff, spec;
	
	//light source 1
	diff = max(dot(fNormal, normalize(vec4(lightPos1, 1.) - fEye)), 0.0);
	spec = max(dot(normalize(-fEye), reflect(-normalize(vec4(lightPos1, 1.) - fEye), fNormal)), 0.0);

	FragColor += kD * diff * (0.2 * vec4(lightColor1, 1.) + 0.8 * fColor);
	FragColor += kS * pow(spec, 2) * (0.2 * vec4(lightColor1, 1.) + 0.8 * vec4(1., 1., 1., 1.));
	
	//light source 2
	diff = max(dot(fNormal, normalize(vec4(lightPos2, 1.) - fEye)), 0.0);
	spec = max(dot(normalize(-fEye), reflect(-normalize(vec4(lightPos2, 1.) - fEye), fNormal)), 0.0);

	FragColor += kD * diff * (0.2 * vec4(lightColor2, 1.) + 0.8 * fColor);
	FragColor += kS * pow(spec, 2) * (0.2 * vec4(lightColor2, 1.) + 0.8 * vec4(1., 1., 1., 1.));
}
