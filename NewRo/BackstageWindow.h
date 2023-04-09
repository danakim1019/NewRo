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
	camera cam;

	BackstageWindow(int m_width, int m_height, int windowWidth, int windowHeight);
	void SetWindowSize(int m_width, int m_height, int xPos, int yPos, int m_windowWidth, int m_windowHeight);
	void SetViewport(int m_width, int m_height);
	void DrawBackstageWindow(GLFWwindow* window, int m_width, int m_height);

	//create BuiltIn Object
	void createBuiltInOBJ(int BuiltInType);

	unsigned int getObjectNum() {
		return Hierachy->objectNum;
	}

	std::string getObjectName(int id) {
		return Hierachy->activeOBJList[id]->name;
	}

	int getObjectID(int id) {
		return Hierachy->activeOBJList[id]->id;
	}

protected:
	HierarchyWindow* Hierachy;
private:
	~BackstageWindow() {}

	void setupBuffer();

	int backstageWidth, backstageHeight;
	int backstageXPos, backstageYPos;
	int windowWidth, windowHeight;

	float m_ratio;
	float fovy;
	ModelView m_model;

	Grid* grid;
	

	BuiltInCylinder* m_cylinder;
	BuiltInCube* m_cube;
	BuiltInSphere* m_sphere;
};

#endif