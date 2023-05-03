#version 430

layout(location=0) in vec3 VertexPosition;
layout(location=1) in vec3 VertexNormal;

out vec3 Position;
out vec3 Normal;
out vec4 FragPosLightSpace;
out vec3 FragOut;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelMatrix;
uniform mat4 MVP;
uniform mat4 lightSpaceMatrix;
uniform bool hasColor;


void main(void)
{
   //FragOut = vec3(ModelMatrix*vec4(VertexPosition,1.0));
   FragOut = vec3(ModelMatrix*vec4(VertexPosition,1.0));
   Normal = normalize(NormalMatrix*VertexNormal);
   Position = (ModelViewMatrix * vec4(VertexPosition,1.0)).xyz;
   FragPosLightSpace = lightSpaceMatrix*vec4(FragOut,1.0);

   gl_Position = MVP*vec4(VertexPosition,1.0);
}