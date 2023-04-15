#ifndef _BASIC_COMPONENT_
#define _BASIC_COMPONENT_

//Component list

#include<iostream>

#include<vector>
#include<math.h>

#include <string>

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

class OBJTransform {
public:
	OBJTransform() {
		pos = { 0,0,0 };
		rot = { 0, 0, 0 };
		scale = { 1, 1, 1 };
	}

	struct Position {
		float x;
		float y;
		float z;
	};
	Position pos;

	struct Rotation {
		float x;
		float y;
		float z;
	};
	Rotation rot;

	struct Scale {
		float x;
		float y;
		float z;
	};
	Scale scale;

	glm::vec3 getPositon() {
		return glm::vec3(pos.x, pos.y, pos.z);
	}

	void setPosition(float x, float y, float z) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
	}

	glm::vec3 getRotation() {
		return glm::vec3(rot.x, rot.y, rot.z);
	}

	void setRotation(float x, float y, float z) {
		rot.x = x;
		rot.y = y;
		rot.z = z;
	}

	glm::vec3 getScale() {
		return glm::vec3(scale.x, scale.y, scale.z);
	}

	void setScale(float x, float y, float z) {
		scale.x = x;
		scale.y = y;
		scale.z = z;
	}

};

class OBJect : public OBJTransform {
public:
	virtual void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location, glm::vec3 lightPosition) {}
	virtual void RenderPicking(){}
	void OutlineRender(){

	}

	std::string name;
	unsigned int id = 0;
	std::string objectType;
};

#endif