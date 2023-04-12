#ifndef _BACKSTAGE_WINDOW_H_
#define _BACKSTAGE_WINDOW_H_

#include<iostream>

#include"Grid.h"
#include"Camera.h"
#include"ModelView.h"

#include"Hierarchy.h"

#include"Loader.h"

class BackstageWindow {
public:
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
	void renderPhase();

	camera cam;

	BackstageWindow(int m_width, int m_height, int windowWidth, int windowHeight);
	void SetWindowSize(int m_width, int m_height, int xPos, int yPos, int m_windowWidth, int m_windowHeight);
	void SetViewport(int m_width, int m_height);
	void DrawBackstageWindow(int m_width, int m_height);

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

protected:
	HierarchyWindow* Hierachy;
	GLFWwindow* window;
private:
	~BackstageWindow() {}

	void setupBuffer();
	bool FBOInit();

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
};

#endif