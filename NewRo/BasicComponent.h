#ifndef _BASIC_COMPONENT_
#define _BASIC_COMPONENT_

//Component list

#include<iostream>

#include<vector>
#include<math.h>

#include <string>
#include"Loader.h"

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include"Component/Material.h"
#include"ImGUI/imgui_impl_opengl3_loader.h"

class Shadow {
public:
	bool bIsShadow;
	int mShadowType;
	GLuint mShadowGLuint;

	Shadow() {
		mShadowType = 0;
		bIsShadow = true;
		mShadowGLuint = 0;
	}
};

class Animation {
public:
	bool bHasAnimation;
	float mAnimationTime;
	int mAnimationNum;

	Animation() {
		bHasAnimation = false;
		mAnimationTime = 0;
		mAnimationNum = 0;
	}
};

class OBJTransform {
public:
	OBJTransform() {
		mPos = { 0,0,0 };
		mRot = { 0, 0, 0 };
		mScale = { 1, 1, 1 };
	}

	struct Position {
		float x;
		float y;
		float z;
	};
	Position mPos;

	struct Rotation {
		float x;
		float y;
		float z;
	};
	Rotation mRot;

	struct Scale {
		float x;
		float y;
		float z;
	};
	Scale mScale;

	glm::vec3 getPositon() {
		return glm::vec3(mPos.x, mPos.y, mPos.z);
	}

	void setPosition(float x, float y, float z) {
		mPos.x = x;
		mPos.y = y;
		mPos.z = z;
	}

	glm::vec3 getRotation() {
		return glm::vec3(mRot.x, mRot.y, mRot.z);
	}

	void setRotation(float x, float y, float z) {
		mRot.x = x;
		mRot.y = y;
		mRot.z = z;
	}

	glm::vec3 getScale() {
		return glm::vec3(mScale.x, mScale.y, mScale.z);
	}

	void setScale(float x, float y, float z) {
		mScale.x = x;
		mScale.y = y;
		mScale.z = z;
	}

};

class OBJect : public OBJTransform {
public:
	virtual void RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location, 
		glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace,Shadow* shadowType, Animation* animation) {}
	virtual void RenderPicking(){}
	virtual void RenderRTShadow(ShaderProgram* shader) {}

	std::string mName;
	unsigned int mID = 0;
	bool isAnim = false;
	std::string mObjectType;
	std::string mShaderType;

	Material* mMat;
};

#endif