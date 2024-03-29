#version 330 core

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ShadowMatrix;

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 UV;

out vec4 fEye;
out vec4 fNormal;
out vec4 fColor;
out vec2 UVcoords;
out vec4 fShadow;
out vec4 fWorld;

void main(){
      gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vPosition, 1.0);
      vec4 world_pos = ModelMatrix * vec4(vPosition, 1.0);
      fEye = ViewMatrix * ModelMatrix * vec4(vPosition, 1.0);
      fNormal = normalize(ViewMatrix * ModelMatrix * vec4(vNormal, 0.));
      fColor = vec4(vColor, 1.0);
      UVcoords = UV;
      fWorld = vec4(vPosition, 1.0);
      fShadow = ShadowMatrix * ModelMatrix * vec4(vPosition, 1.0);
}
