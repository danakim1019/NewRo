#include"BackstageWindow.h"

BackstageWindow::BackstageWindow(int m_width, int m_height,int w_Width,int w_Height) :backstageWidth(m_width), backstageHeight(m_height) , windowWidth(w_Width),windowHeight(w_Height)
{ 
	fovy = 45.0f; 
	m_ratio = backstageWidth / static_cast<float>(backstageHeight);
	setupPickingShader();
	setupBuffer();
}

void BackstageWindow::setupPickingShader() {
	pickingShaderProgram = new ShaderProgram();
	pickingShaderProgram->initFromFiles("Shader/picking.vert", "Shader/picking.frag");

	pickingShaderProgram->addAttribute("coord3d");

	pickingShaderProgram->addUniform("MVP");
	pickingShaderProgram->addUniform("gModelIndex");
	pickingShaderProgram->addUniform("gDrawIndex");

}

BackstageWindow::PixelInfo BackstageWindow::ReadPixel(unsigned int x, unsigned int y) {
	//프레임 버퍼에서 데이터 획득
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);		//제일 먼저 저장해놓은 오브젝트 인덱스(아이디) 가져옴

	//픽셀 데이터 읽기
	PixelInfo Pixel;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &Pixel);

	//버퍼를 해제
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	return Pixel;
}

int BackstageWindow::selectObject(int cx, int cy,int selectedObjIndex) {
	//PixelInfo Pixel = ReadPixel(cx, backstageHeight - cy - 1);
	PixelInfo Pixel = ReadPixel(cx, windowHeight - cy - 1);
	//PixelInfo Pixel = ReadPixel(cx, cy);
	//std::cout << cx << "," << cy <<  std::endl;
	printf("%.1f, %.1f, %.1f\n", Pixel.drawID, Pixel.objectID, Pixel.primID);

	//if (Pixel.objectID != 0 && Pixel.objectID != 0.2f&&selectedObjIndex==-1) {
	if (Pixel.objectID != 0 && Pixel.objectID != 0.2f) {
		return Pixel.objectID;
	}
	else {
		return -1;
	}
}

void BackstageWindow::DrawBackstageWindow(int m_width, int m_height) {
	//glViewport(0, 0, display_w, display_h);
  /* glViewport(0, 0, 200, 200);
   glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
   glClear(GL_COLOR_BUFFER_BIT);*/
	

	glClearColor(0.0f, 0.0f, 0.0f, 0);		//background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear up color and depth buffer

	glEnable(GL_SCISSOR_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 0);		//background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear up color and depth buffer
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_DEPTH_TEST);			//test whether an object is in front of other object?
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glViewport(backstageXPos, backstageYPos, m_width, m_height);
	glScissor(backstageXPos, backstageYPos, backstageWidth, backstageHeight);

	glDisable(GL_SCISSOR_TEST);
   
	viewMat = cam.GetViewMatrix();
	projectionMat = glm::perspective(glm::radians(fovy), m_ratio, 0.1f, 1000.0f);
	modelMat = m_model.getMatrix();

	pickingPhase();
	renderPhase();

}

void BackstageWindow::pickingPhase() {
	//FBO를 바인딩
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pickingShaderProgram->use();
	modelViewArray = new glm::mat4[Hierachy->objectNum];
		for (int i = 0; i < Hierachy->objectNum; i++) {
			m_model.glPushMatrix();
			//Scale
			glm::vec3 scl = getObject(i + 1)->getScale();
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), scl);
			m_model.glScale(scl.x, scl.y,scl.z);
			//Rotate
			glm::vec3 rot = getObject(i + 1)->getRotation();
			m_model.glRotate(rot.z, 0, 0, 1);
			m_model.glRotate(rot.y, 0, 1, 0);
			m_model.glRotate(rot.x, 1, 0, 0);
			//Translate
			glm::vec3 position = getObject(i+1)->getPositon();
			m_model.glTranslate(position.x,position.y,position.z);

			modelMat = m_model.getMatrix();
			modelViewArray[i] = modelMat;

			glm::mat4 mMVP = projectionMat * viewMat * modelMat;

			glUniform1ui(pickingShaderProgram->uniform("gModelIndex"), getObjectID(i+1));
			//std::cout << "getObjectID(i):" << getObjectID(i+1) << std::endl;
			glUniformMatrix4fv(pickingShaderProgram->uniform("MVP"),1,GL_FALSE,glm::value_ptr(mMVP));
			Hierachy->activeOBJList[i]->RenderPicking();

			m_model.glPopMatrix();
		}
	pickingShaderProgram->disable();


	//FBO를 해제
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void BackstageWindow::renderPhase() {

	//그리드 그리기
	grid->draw(modelMat, viewMat, projectionMat);

	glm::mat4 origin = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	Hierachy->drawList(modelViewArray, viewMat, projectionMat, origin, glm::vec3(0, 30, 0));

	/*m_cube->draw(modelMat, viewMat, projection, origin,cam.Position);
	origin = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f));
	m_cylinder->draw(modelMat, viewMat, projection,origin,0,0,0);
	origin = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, 0.0f));
	m_sphere->draw(modelMat, viewMat, projection, origin, glm::vec3(0, 30, 0));*/
	
}

void BackstageWindow::createBuiltInOBJ(int BuiltInType) {
	Hierachy->createOBJ(BuiltInType);
}

void BackstageWindow::SetWindowSize(int m_width, int m_height, int xPos, int yPos,int m_windowWidth,int m_windowHeight) {
	backstageWidth = m_width-4;
	backstageHeight = m_height-22;
	backstageXPos = xPos+2;
	backstageYPos = yPos+4;
	windowWidth = m_windowWidth;
	windowHeight = m_windowHeight;
	glViewport(backstageXPos, backstageYPos, m_width, m_height);

	FBOInit();
}

void BackstageWindow::SetViewport(int m_width, int m_height) {
	std::cout << "SetViewport" << std::endl;
	//glViewport(backstageXPos, backstageYPos, m_width, m_height);
}

void BackstageWindow::setupBuffer() {
	cam = camera(glm::vec3(0.0f, 30.0f, 30.0f));
	Hierachy = new HierarchyWindow();
	grid = new Grid();
	//Hierachy->createOBJ(1);
	/*m_cylinder = new BuiltInCylinder();
	m_cube = new BuiltInCube(0);
	m_sphere = new BuiltInSphere();*/

	FBOInit();
}

bool BackstageWindow::FBOInit() {
	//Create the FBO
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	//Create the texture object for the primitive information buffer
	glGenTextures(1, &m_pickingTexture);
	glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB32F, backstageWidth, backstageHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pickingTexture, 0);
	
	//Create the texture object for the depth buffer
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	//glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT, backstageWidth, backstageHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	//Disable reading to avoid porblems with older GPUs
	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	//Verify that the FBO is correct
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return -1;
	}

	//Restore the default framebuffer
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return glGetError() == GL_NO_ERROR;

}