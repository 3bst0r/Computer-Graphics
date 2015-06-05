#version 330 core

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec3 vNormal;

out vec4 fEye;
out vec4 fNormal;
out vec4 fColor;

void main(){
   gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
   fEye = ViewMatrix * ModelMatrix * vec4(vPosition, 1.0);
   fNormal = normalize(ViewMatrix * ModelMatrix * vNormal);
   fColor = vec4(vColor.r, vColor.g, vColor.b, 1.0);
}
