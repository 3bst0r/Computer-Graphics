#version 330 core

const int max_lights = 2;

in vec4 fColor;
in vec4 fEye;
in vec4 fNormal;

out vec4 FragColor;

uniform vec3 lightPos1;
uniform vec3 lightColor1;
uniform vec3 lightPos2;
uniform vec3 lightColor2;

void main()
{
	vec4 ambientColor = 0.5 * fColor;

    FragColor = ambientColor;

	vec4 s1 = normalize(vec4(lightPos1, 1.) - fEye);
	vec4 r1 = reflect(-s1, fNormal);
	vec4 v1 = normalize(-fEye);
	float spec1 = max(dot(v1, r1), 0.0);
	float diff1 = max(dot(fNormal,s1), 0.0);
	
	vec4 diffColor1 = 0.7 * diff1 * fColor;
	vec4 specColor1 = vec4(pow(spec1, 3) * lightColor1, 1.);
	
	vec4 s2 = normalize(vec4(lightPos2, 1.) - fEye);
	vec4 r2 = reflect(-s2, fNormal);
	vec4 v2 = normalize(-fEye);
	float spec2 = max(dot(v2,r2), 0.0);
	float diff2 = max(dot(fNormal,s2), 0.0);
	
	vec4 diffColor2 = 0.7 * diff2 * fColor;
	vec4 specColor2 = vec4(pow(spec2, 3) * lightColor2, 1.);
		
	FragColor += diffColor1 + specColor1 + diffColor2 + specColor2;
}
