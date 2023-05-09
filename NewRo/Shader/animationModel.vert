#version 450

layout(location=0) in vec3 VertexPosition;
layout(location=1) in vec3 VertexNormal;
layout(location=2) in vec2 VertexColor;
layout(location=3) in ivec4 BoneIDs;
layout(location=4) in vec4 Weights;


out vec3 Position;
out vec3 Normal;
out vec2 Color;
out vec4 FragPosLightSpace;
out vec3 FragOut;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 MVP;
uniform mat3 NormalMatrix;

uniform mat4 lightSpaceMatrix;

uniform bool hasColor;

uniform mat4 gBones[100];

uniform mat4 shadowMatrix;

uniform uint character;
uniform bool isShadow;

void main(void)
{

   if(hasColor){
		Color = VertexColor;
	}

	mat4 BoneTransform = mat4(1.0);
	BoneTransform=gBones[BoneIDs[0]]*Weights[0];
	BoneTransform += gBones[BoneIDs[1]] * Weights[1];
	BoneTransform += gBones[BoneIDs[2]] * Weights[2];
	BoneTransform += gBones[BoneIDs[3]] * Weights[3];

	vec4 PosL = BoneTransform * vec4(VertexPosition,1.0);

	FragOut = (ModelMatrix*PosL).xyz;
	Position = (ModelViewMatrix * PosL).xyz;
    FragPosLightSpace = lightSpaceMatrix*vec4(FragOut,1.0);
	Normal = normalize(NormalMatrix*VertexNormal);
	gl_Position = MVP*PosL;

}