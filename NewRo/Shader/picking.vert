#version 430

layout(location=0) in vec3 coord3d;

uniform mat4 MVP;


void main(void)
{
   gl_Position = MVP * vec4( coord3d,1.0);
}