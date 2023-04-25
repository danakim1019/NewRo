#pragma once

#include"../BasicComponent.h"

enum class ShadowType {
	HARD_SHADOW = 0,
	INTERPOLED_SHADOW = 1,
	PCF_SHADOW = 2,
	INTERPOLED_PCF_SHADOW = 3,
	VSM_SHADOW = 4,
	FILTERED_VSM_SHADOW = 5
};

class Light :public OBJect{
public:
	glm::vec3 LightColor;
	float LightIntensity;

	Light(float xPos,float yPos, float zPos)
	{
		setPosition(xPos, yPos, zPos);
		name = "Light";
	}
};