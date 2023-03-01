#ifndef _BASIC_COMPONENT_
#define _BASIC_COMPONENT_

//Component list

#include<iostream>

#include<vector>
#include<math.h>
#include"Loader.h"

#include <string>

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