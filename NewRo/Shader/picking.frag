#version 430

uniform uint gDrawIndex;
uniform uint gModelIndex;

out vec3 FragColors;

void main()
{
   FragColors = vec3(float(gModelIndex),float(gDrawIndex),float(gl_PrimitiveID+1));
}