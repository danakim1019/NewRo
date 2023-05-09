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

	void setupPickingShader();
	GLuint m_fbo,m_pickingTexture,m_depthTexture;
	PixelInfo ReadPixel(unsigned int x, unsigned int y);
	int selectObject(int cx, int cy,int selectedObjIndex);
	ShaderProgram* pickingShaderProgram;
	void pickingPhase();

	void guizmoPhase(int selectedObjID);

	//Outline
	ShaderProgram* outlineShaderProgram;
	GLuint m_outfbo;
	void outlinePhase(int selectedObjID);
	
	//Shadow
	ShaderProgram* shadowShaderProgram;
	GLuint m_shadowfbo;

	GLuint shadowMap;

	glm::mat4 lightProjection;
	glm::mat4 lightSpace;
	glm::mat4 lightView;

	void renderPhase(Shadow* shadow, Animation* animation, float deltaTime);

	//Animation
	void animationPhase(float animationTime, unsigned int st, unsigned int num, unsigned int shadow);
	vector<glm::mat4> Transforms;
	vector<glm::fdualquat> dualQuaternions;
	vector<glm::mat2x4> DQs;

	vector<glm::mat4> Transforms2;
	vector<glm::mat4> Transforms3;

	ShaderProgram* animShaderProgram;

	//vector<glm::fdualquat> dualQuaternions2;
	//vector<glm::mat2x4> DQs2;


	camera cam;

	BackstageWindow(int m_width, int m_height, int windowWidth, int windowHeight);
	void SetWindowSize(int m_width, int m_height, int xPos, int yPos, int m_windowWidth, int m_windowHeight);
	void SetViewport(int m_width, int m_height);
	void DrawBackstageWindow(int m_width, int m_height, int selectedObjID, float deltaTime);

	//create BuiltIn Object
	void createBuiltInOBJ(int BuiltInType);

	unsigned int getObjectNum() {
		return Hierachy->objectNum;
	}

	std::string getObjectName(unsigned int id) {
		return Hierachy->activeOBJList[id-1]->name;
	}

	unsigned int getObjectID(unsigned int id) {
		return Hierachy->activeOBJList[id-1]->id;
	}

	OBJect* getObject(unsigned int id) {
		return Hierachy->activeOBJList[id-1];
	}

	void setObjectName(char* name,unsigned int id) {
		Hierachy->activeOBJList[id-1]->name = name;
	}

	Shadow* getShadow(int lightOBJID) {
		return ((Light*)Hierachy->activeOBJList[lightOBJID])->shadow;
	}

	void setShadow(bool isShadow, int shadowType) {
		((Light*)Hierachy->activeOBJList[0])->shadow->isShadow = isShadow;
		((Light*)Hierachy->activeOBJList[0])->shadow->shadowType = shadowType;
	}

	bool initializeShadowMap();

protected:
	HierarchyWindow* Hierachy;
	GLFWwindow* window;
	glm::mat4* modelViewArray;
private:
	~BackstageWindow() {}

	void setupBuffer();
	void generateShadowMap(glm::mat4 lightSpace, Shadow* shadow);
	bool generateOutlineMap();


	int backstageWidth, backstageHeight;
	int backstageXPos, backstageYPos;
	int windowWidth, windowHeight;

	float m_ratio;
	float fovy;
	ModelView m_model;

	glm::mat4 viewMat;
	glm::mat4 projectionMat;
	glm::mat4 modelMat;

	Grid* grid;

	BuiltInCylinder* m_cylinder;
	BuiltInCube* m_cube;
	BuiltInSphere* m_sphere;

	float angle;
};

#endif