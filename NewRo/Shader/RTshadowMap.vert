#version 430

layout(location=0) in vec3 coord3d;
layout(location=3) in ivec4 BoneIDs;
layout(location=4) in vec4 Weights;

out vec4 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 Model;

uniform mat4 ViewMatrix;

uniform mat4 gBones[100];

void main(void)
{
	mat4 BoneTransform = mat4(1.0);
	BoneTransform = gBones[BoneIDs[0]]*Weights[0];
	BoneTransform += gBones[BoneIDs[1]]*Weights[1];
	BoneTransform += gBones[BoneIDs[2]]*Weights[2];
	BoneTransform += gBones[BoneIDs[3]]*Weights[3];

	vec4 PosL = BoneTransform * vec4(coord3d,1.0);

   gl_Position = lightSpaceMatrix* Model *PosL;
   position = gl_Position;
}