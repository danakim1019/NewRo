#version 450 core


layout(location = 0) in vec3 coord3d;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoord;
out vec3 fNormal;
out vec3 fPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix; 
uniform mat4 location;


void main() {

   fNormal = normalize(NormalMatrix * v_normal); //local 좌표계에서의 normal을 변경
   vec4 pos = ModelViewMatrix*vec4(coord3d,1.0);
   fPosition = pos.xyz;
   TexCoord = aTexCoords;

   //gl_Position = projection*view*location*model * vec4(aPos, 1.0f);
   gl_Position = projection*view*location *model* vec4(coord3d, 1.0);
   //gl_Position = projection*view*location*model* vec4(aPos, 1.0f);

}