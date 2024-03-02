// https://github.com/danakim1019/NewRo
// The MIT License(MIT)
//
//Copyright (C) 2023 by danakim1019
// 
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 

#ifndef _BACKSTAGE_WINDOW_H_
#define _BACKSTAGE_WINDOW_H_

#include<iostream>

#include"Grid.h"
#include"Camera.h"
#include"ModelView.h"

#include"Hierarchy.h"

#include "ImGUI/imgui.h"
#include"ImGUI/imgui_internal.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include"ImGUI/ImGuizmo.h"

#include"Loader.h"

static bool useSnap = false;
static float snap[3] = { 1.f, 1.f, 1.f };
static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
static bool boundSizing = false;
static bool boundSizingSnap = false;

class BackstageWindow {
public:

	camera mCam;

	ImGuizmo::OPERATION mCurrentGizmoOperation;
	ImGuizmo::MODE mCurrentGizmoMode;

	struct PixelInfo {
		float objectID;
		float drawID;
		float primID;

		PixelInfo() {
			objectID = 0.0f;
			drawID = 0.0f;
			primID = 0.0f;
		}
	};

	
	BackstageWindow(int m_width, int m_height, int windowWidth, int windowHeight);
	~BackstageWindow();

	void SetWindowSize(int width, int height, int xPos, int yPos, int m_windowWidth, int m_windowHeight);
	void SetViewport(int width, int height);
	void DrawBackstageWindow(int width, int height, int selectedObjID, float deltaTime);

	int SelectObject(int cx, int cy, int selectedObjIndex);

	//Create BuiltIn Object
	void CreateBuiltInOBJ(int builtInType);

	//Getter Setter
	unsigned int getObjectNum() {return mHierachy->objectNum;}
	std::string getObjectName(unsigned int id) {return mHierachy->activeOBJList[id-1]->mName;	}
	unsigned int getObjectID(unsigned int id) {return mHierachy->activeOBJList[id-1]->mID;	}
	OBJect* getObject(unsigned int id) {	return mHierachy->activeOBJList[id-1];	}
	Shadow* getShadow(int lightOBJID) { return ((Light*)mHierachy->activeOBJList[lightOBJID])->shadow; }

	void setObjectName(char* name,unsigned int id) {
		mHierachy->activeOBJList[id-1]->mName = name;
	}

	void setShadow(bool isShadow, int shadowType) {
		((Light*)mHierachy->activeOBJList[0])->shadow->bIsShadow = isShadow;
		((Light*)mHierachy->activeOBJList[0])->shadow->mShadowType = shadowType;
	}

protected:
	HierarchyWindow* mHierachy;
	GLFWwindow* mWindow;
	glm::mat4* mModelViewArray;
private:
	void setupPickingShader();
	GLuint m_fbo, mPickingTexture, mDepthTexture;
	PixelInfo ReadPixel(unsigned int x, unsigned int y);
	ShaderProgram* mPickingShaderProgram;

	// Note : Order of Phase function restrictly. Please check before change
	void pickingPhase();

	// Note : Order of Phase function restrictly. Please check before change
	void guizmoPhase(int selectedObjID);

	//Outline
	ShaderProgram* mOutlineShaderProgram;
	GLuint mOutfbo;

	// Note : Order of Phase function restrictly. Please check before change
	void outlinePhase(int selectedObjID);

	//Shadow
	ShaderProgram* mShadowShaderProgram;
	ShaderProgram* mRTShadowShaderProgram;
	GLuint mShadowfbo;
	GLuint mShadowMap;

	glm::mat4 mLightProjection;
	glm::mat4 mLightSpace;
	glm::mat4 mLightView;

	// Note : Order of Phase function restrictly. Please check before change
	void renderPhase(Shadow* shadow, Animation* animation, float deltaTime);

	std::vector<glm::mat4> mTransforms;
	std::vector<glm::fdualquat> mDualQuaternions;
	std::vector<glm::mat2x4> mDQs;

	ShaderProgram* mAnimShaderProgram;


	bool initializeShadowMap();

	// initialize basic objects
	void setupBuffer();

	// Note : Order of generate shadowmap function restrictly. Please check before use
	void generateShadowMap(glm::mat4 lightSpace, Shadow* shadow, Animation* animation);
	bool generateOutlineMap();

	// Change when windowsizecallback called
	int mBackstageWidth, mBackstageHeight;
	int mBackstageXPos, mBackstageYPos;
	int mWindowWidth, mWindowHeight;

	float mRatio;
	float mFovy;

	ModelView mModel;

	glm::mat4 mViewMat;
	glm::mat4 mProjectionMat;
	glm::mat4 mModelMat;

	//Skybox
	Skybox* mSkybox;

	//Grid
	Grid* mGrid;

	// Basic Objects for test
	BuiltInCylinder* mCylinder;
	BuiltInCube* mCube;
	BuiltInSphere* mSphere;

	float mAngle;

	glm::mat4 origin;
};

#endif