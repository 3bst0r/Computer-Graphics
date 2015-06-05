#version 330 core

const int max_lights = 2;

in vec4 fColor;
in vec4 fEye;
in vec4 fNormal;

out vec4 FragColor;

//uniform vec4 lightPos;
//uniform vec3 lightColor;

void main()
{
	vec4 ambientColor = 0.5 * fColor;

    FragColor = ambientColor;
    
	//for(int i = 0; i < max_lights; i++){
		vec4 lightPos = vec4(0.0, 0.0, -10.0, 1.0);
		vec3 lightColor = vec3(1.0, 1.0, 1.0);
		vec4 s = normalize(lightPos - fEye);
		vec4 r = reflect(-s, fNormal);
		vec4 v = normalize(-fEye);
		float spec = max(dot(v,r), 0.0);
		float diff = max(dot(fNormal,s), 0.0);
		
		vec4 diffColor = 0.7 * diff * fColor;
		vec4 specColor = vec4(pow(spec, 3) * lightColor, 1.);
		
		FragColor += diffColor + specColor;
	//}
}
