#include"BackstageWindow.h"


void BackstageWindow::DrawBackstageWindow(GLFWwindow* window,int m_width, int m_height) {
    //glViewport(0, 0, display_w, display_h);
   /* glViewport(0, 0, 200, 200);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);*/
	glClearColor(0.0f, 0.0f, 0.0f, 0);		//background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear up color and depth buffer

	glViewport(0, 0, m_width, m_height);
	glScissor(backstageXPos, backstageYPos, backstageWidth, backstageHeight);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);			//test whether an object is in front of other object?

	glClearColor(0.5f, 0.5f, 0.5f, 0);		//background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear up color and depth buffer
	//glEnable(GL_CLIP_DISTANCE0);
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_SCISSOR_TEST);
	


}

void BackstageWindow::SetWindowSize(int m_width, int m_height, int xPos, int yPos,int m_windowWidth,int m_windowHeight) {
	backstageWidth = m_width-4;
	backstageHeight = m_height-22;
	backstageXPos = xPos+2;
	backstageYPos = yPos+4;
	windowWidth = m_windowWidth;
	windowHeight = m_windowHeight;
}

void BackstageWindow::SetViewport(int m_width, int m_height) {
	glViewport(0, 0, m_width, m_height);
}

void BackstageWindow::setupBuffer() {
	grid = new Grid();
}