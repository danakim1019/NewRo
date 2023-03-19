#ifndef _BASIC_COMPONENT_
#define _BASIC_COMPONENT_

//Component list

#include<iostream>

#include<vector>
#include<math.h>
//#include"Loader.h"

#include <string>

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

class OBJTransform {
public:
	OBJTransform() {
		Position = new float[3]{ 0,0,0 };
		Rotation = new float[3] { 0, 0, 0 };
		Scale = new float[3] { 0, 0, 0 };
	}

	float* Position;
	float* Rotation;
	float* Scale;
};

#endif