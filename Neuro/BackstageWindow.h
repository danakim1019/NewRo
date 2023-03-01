#ifndef _BACKSTAGE_WINDOW_H_
#define _BACKSTAGE_WINDOW_H_

#include<iostream>

#include"Grid.h"
#include"Camera.h"

#include <GLFW/glfw3.h> // Will drag system OpenGL headers


class BackstageWindow {
public:
	BackstageWindow(int m_width,int m_height):backstageWidth(m_width),backstageHeight(m_height) {}
	void SetWindowSize(int m_width, int m_height, int xPos, int yPos, int m_windowWidth, int m_windowHeight);
	void SetViewport(int m_width, int m_height);
	void DrawBackstageWindow(GLFWwindow* window, int m_width, int m_height);
private:
	~BackstageWindow() {}

	void setupBuffer();

	int backstageWidth, backstageHeight;
	int backstageXPos, backstageYPos;
	int windowWidth, windowHeight;


	Grid* grid;
};

#endif