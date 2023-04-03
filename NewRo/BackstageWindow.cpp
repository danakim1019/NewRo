#include"BackstageWindow.h"

BackstageWindow::BackstageWindow(int m_width, int m_height,int w_Width,int w_Height) :backstageWidth(m_width), backstageHeight(m_height) , windowWidth(w_Width),windowHeight(w_Height)
{ 
	fovy = 45.0f; 
	m_ratio = backstageWidth / static_cast<float>(backstageHeight);
	setupBuffer();
}

void BackstageWindow::DrawBackstageWindow(GLFWwindow* window,int m_width, int m_height) {

    //glViewport(0, 0, display_w, display_h);
   /* glViewport(0, 0, 200, 200);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);*/
	glClearColor(0.0f, 0.0f, 0.0f, 0);		//background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear up color and depth buffer

	glViewport(backstageXPos, backstageYPos, m_width, m_height);
	glScissor(backstageXPos, backstageYPos, backstageWidth, backstageHeight);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);			//test whether an object is in front of other object?

	glClearColor(0.5f, 0.5f, 0.5f, 0);		//background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear up color and depth buffer
	//glEnable(GL_CLIP_DISTANCE0);
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_SCISSOR_TEST);
	

	glm::mat4 viewMat = cam.GetViewMatrix();

	glm::mat4 projection = glm::perspective(glm::radians(fovy), m_ratio, 0.1f, 1000.0f);

	glm::mat4 model;
	m_model.glPushMatrix();
	model = m_model.getMatrix();

	//그리드 그리기
	grid->draw(model,viewMat,projection);

	glm::mat4 origin = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	m_cylinder->draw(model, viewMat, projection,origin,0,0,0);

	m_model.glPopMatrix();
}

void BackstageWindow::SetWindowSize(int m_width, int m_height, int xPos, int yPos,int m_windowWidth,int m_windowHeight) {
	backstageWidth = m_width-4;
	backstageHeight = m_height-22;
	backstageXPos = xPos+2;
	backstageYPos = yPos+4;
	windowWidth = m_windowWidth;
	windowHeight = m_windowHeight;
	glViewport(backstageXPos, backstageYPos, m_width, m_height);
}

void BackstageWindow::SetViewport(int m_width, int m_height) {
	std::cout << "SetViewport" << std::endl;
	//glViewport(backstageXPos, backstageYPos, m_width, m_height);
}

void BackstageWindow::setupBuffer() {
	cam = camera(glm::vec3(0.0f, -100.0f, 100.0f));
	grid = new Grid();
	m_cylinder = new cylinder();
}