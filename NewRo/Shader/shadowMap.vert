#version 430

layout(location=0) in vec3 coord3d;

out vec4 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 Model;


void main(void)
{
   gl_Position = lightSpaceMatrix* Model * vec4( coord3d,1.0);
   position = gl_Position;
}