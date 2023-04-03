#ifndef _BACKSTAGE_WINDOW_H_
#define _BACKSTAGE_WINDOW_H_

#include<iostream>

#include"Grid.h"
#include"Camera.h"
#include"ModelView.h"
#include"3DObjects/BuiltInCylinderObj.h"

#include"Loader.h"


class BackstageWindow {
public:
	camera cam;

	BackstageWindow(int m_width, int m_height, int windowWidth, int windowHeight);
	void SetWindowSize(int m_width, int m_height, int xPos, int yPos, int m_windowWidth, int m_windowHeight);
	void SetViewport(int m_width, int m_height);
	void DrawBackstageWindow(GLFWwindow* window, int m_width, int m_height);
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
	cylinder* m_cylinder;
};

#endif