#version 330 core

uniform mat4 MVP_matrix;
uniform mat4 ModelMatrix;

layout (location = 0) in vec3 vPosition;

   void main() {
      gl_Position = MVP_matrix * ModelMatrix *  vec4(vPosition, 1.0);
   }
