#include"BackstageWindow.h"

const unsigned int SHADOW_WIDTH = 2028, SHADOW_HEIGHT = 2028;

BackstageWindow::BackstageWindow(int m_width, int m_height,int w_Width,int w_Height) :backstageWidth(m_width), backstageHeight(m_height) , windowWidth(w_Width),windowHeight(w_Height)
{ 
	fovy = 45.0f; 
	m_ratio = backstageWidth / static_cast<float>(backstageHeight);
	angle = 0;
	lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, -80.0f, 80.0f);

	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	mCurrentGizmoMode = ImGuizmo::LOCAL;

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

	outlineShaderProgram = new ShaderProgram();
	outlineShaderProgram->initFromFiles("Shader/picking.vert", "Shader/Outline.frag");

	outlineShaderProgram->addAttribute("coord3d");

	outlineShaderProgram->addUniform("MVP");
	outlineShaderProgram->addUniform("gModelIndex");
	outlineShaderProgram->addUniform("gDrawIndex");

	shadowShaderProgram = new ShaderProgram();
	shadowShaderProgram->initFromFiles("Shader/shadowMap.vert", "Shader/shadowMap.frag");
	shadowShaderProgram->addUniform("lightSpaceMatrix");
	shadowShaderProgram->addUniform("Model");
	
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

	PixelInfo Pixel = ReadPixel(cx, windowHeight - cy - 1);
	printf("%.1f, %.1f, %.1f\n", Pixel.drawID, Pixel.objectID, Pixel.primID);

	int result;
	(Pixel.objectID != 0 && Pixel.objectID != 0.2f) ? result= Pixel.objectID : result= -1;
	return result;
}

void BackstageWindow::DrawBackstageWindow(int m_width, int m_height, int selectedObjID) {

	//std::cout << "***********************************************" << std::endl;
	std::cout << "selectedObjID: " << selectedObjID << std::endl;
	//std::cout << "***********************************************" << std::endl;
	std::cout << "(Light*)Hierachy->activeOBJList[0])->shadow->isShadow:" << (
		(Light*)Hierachy->activeOBJList[0])->shadow->isShadow << std::endl;


	SetViewport(m_width,m_height);
   
	viewMat = cam.GetViewMatrix();
	projectionMat = glm::perspective(glm::radians(fovy), m_ratio, 0.1f, 1000.0f);
	modelMat = m_model.getMatrix();

	if (angle == 360)
		angle = 0;
	else
		angle += 0.01;
	float radius = 20;
	float x = glm::sin(angle) * radius;
	float z = glm::cos(angle) * radius;
	Hierachy->activeOBJList[0]->setPosition(x, 10, z);

	Hierachy->activeOBJList[2]->setPosition(0, -3, 0);
	Hierachy->activeOBJList[2]->setScale(10, 1, 10);
	
	glm::mat4 model;
	lightView = glm::lookAt(Hierachy->activeOBJList[0]->getPositon(), glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
	lightSpace = lightProjection * lightView;

	glm::mat4 origin = glm::mat4(1.0);

	pickingPhase();

	generateShadowMap(lightSpace, ((Light*)Hierachy->activeOBJList[0])->shadow);

	if (selectedObjID > 0) {
		guizmoPhase(selectedObjID);
		outlinePhase(selectedObjID);
	}

	//그리드 그리기
	grid->draw(origin, viewMat, projectionMat);

	renderPhase(((Light*)Hierachy->activeOBJList[0])->shadow);

}

void BackstageWindow::pickingPhase() {
	//FBO를 바인딩
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pickingShaderProgram->use();
	modelViewArray = new glm::mat4[Hierachy->objectNum];
		for (int i = 0; i < Hierachy->objectNum; i++) {
			m_model.glPushMatrix();

			//Translate
			glm::vec3 position = getObject(i+1)->getPositon();
			m_model.glTranslate(position.x,position.y,position.z);
			//Rotate
			glm::vec3 rot = getObject(i + 1)->getRotation();
			m_model.glRotate(rot.z, 0, 0, 1);
			m_model.glRotate(rot.y, 0, 1, 0);
			m_model.glRotate(rot.x, 1, 0, 0);
			//Scale
			glm::vec3 scl = getObject(i + 1)->getScale();
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), scl);
			m_model.glScale(scl.x, scl.y, scl.z);
 
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

void BackstageWindow::guizmoPhase(int selectedObjID) {
		ImGuizmo::SetID(selectedObjID-1);
		//mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		glm::vec3 pos = Hierachy->activeOBJList[selectedObjID - 1]->getPositon();
		float camDistance = glm::length(pos - cam.Position);
		glm::mat4 modelV = glm::translate(glm::mat4(1.0f), pos);

		//ImGuizmo setting
		ImGuiIO& io = ImGui::GetIO();
		float viewManipulateRight = io.DisplaySize.x;
		float viewManipulateTop = 0;
		static ImGuiWindowFlags gizmoWindowFlags = 0;
		//ImGui::Begin("Gizmo", 0, gizmoWindowFlags);
		ImGuizmo::SetDrawlist();

		ImGuizmo::RecomposeMatrixFromComponents((const float*)(&(Hierachy->activeOBJList[selectedObjID - 1]->pos.x))
			, (const float*)(&(Hierachy->activeOBJList[selectedObjID - 1]->rot.x)),
			(const float*)(&(Hierachy->activeOBJList[selectedObjID - 1]->scale.x)),
			(float*)glm::value_ptr(modelViewArray[selectedObjID - 1]));

		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
		viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
		viewManipulateTop = ImGui::GetWindowPos().y;
		//std::cout << "viewManipulateTop: " << viewManipulateTop << std::endl;
		gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(ImVec2(backstageXPos, backstageYPos)
			, ImVec2(backstageXPos + backstageWidth, backstageYPos)) ? ImGuiWindowFlags_NoMove : 0;

		glm::mat4 cameraViewM = cam.GetViewMatrix();

		ImGuizmo::Manipulate((const float*)glm::value_ptr(cameraViewM)
			, (const float*)glm::value_ptr(projectionMat), mCurrentGizmoOperation, mCurrentGizmoMode, (float*)glm::value_ptr(modelViewArray[selectedObjID - 1]), NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

		glm::vec3 Position = cam.Position;
		glm::vec3 Rotation = glm::vec3(cam.Yaw, cam.Pitch,cam.Roll);
		glm::vec3 Scale = glm::vec3(1.0);

		ImGuizmo::RecomposeMatrixFromComponents((const float*)glm::value_ptr(Position)
			, (const float*)glm::value_ptr(Rotation),
			(const float*)glm::value_ptr(Scale), (float*)glm::value_ptr(cameraViewM));

		//
		ImGuizmo::ViewManipulate((float*)glm::value_ptr(cameraViewM), camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

		ImGuizmo::DecomposeMatrixToComponents((const float*)glm::value_ptr(cameraViewM),
			(float*)glm::value_ptr(Position)
			, (float*)glm::value_ptr(Rotation),
			(float*)glm::value_ptr(Scale));

		cam.setCameraSetting(Position, Rotation, Scale);

		ImGuizmo::DecomposeMatrixToComponents((const float*)glm::value_ptr(modelViewArray[selectedObjID - 1]),
			(float*)(&(Hierachy->activeOBJList[selectedObjID - 1]->pos.x))
			, (float*)(&(Hierachy->activeOBJList[selectedObjID - 1]->rot.x)),
			(float*)(&(Hierachy->activeOBJList[selectedObjID - 1]->scale.x))); 

}

void BackstageWindow::outlinePhase(int selectedObjID) {

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		float outlineScale = 1.02f;

		glm::mat4 outlineModel = modelViewArray[selectedObjID - 1];
		outlineModel = glm::scale(outlineModel, glm::vec3(outlineScale, outlineScale, outlineScale));
		

		glm::mat4 mMVP = projectionMat * viewMat * outlineModel;

		outlineShaderProgram->use();

		glUniformMatrix4fv(pickingShaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mMVP));
		Hierachy->activeOBJList[selectedObjID-1]->RenderPicking();

		outlineShaderProgram->disable();

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);

}


void BackstageWindow::renderPhase(Shadow* shadow) {

	glm::mat4 origin = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	Hierachy->drawList(modelViewArray, viewMat, projectionMat, origin, cam.Position, glm::vec3(0, 30, 0), lightSpace, shadow);
}

void BackstageWindow::animationPhase(float animationTime, unsigned int st, unsigned int num, unsigned int shadow)
{
	animShaderProgram->use();
		//반복하도록 설정
		float TimeInTicks = animationTime * 30.0f;

		int type = ((LoadedModelObj*)Hierachy->activeOBJList[0])->BoneTransform(TimeInTicks, Transforms, dualQuaternions);
		type = 0;
		float rate = 0.5;

		vector<glm::fdualquat> dualQuaternions3;
		for (unsigned int i = 0; i < Transforms.size(); i++) {
			glm::fdualquat a = dualQuaternions[i];
			//glm::fdualquat b = dualQuaternions2[i];
			//glm::fdualquat c = glm::lerp(a,b,rate);

			Transforms3[i] = Transforms[i];
			dualQuaternions3.push_back(a);
		}

		glUniformMatrix4fv(animShaderProgram->uniform("gBones[0]"), Transforms3.size(), GL_FALSE, glm::value_ptr(Transforms3[0]));
	
		DQs.resize(dualQuaternions.size());

		for (unsigned int i = 0; i < dualQuaternions.size(); i++) {
			DQs[i] = glm::mat2x4_cast(dualQuaternions3[i]);		//blended result
		}
		glUniformMatrix2x4fv(animShaderProgram->uniform("dqs[0]"),dualQuaternions.size(),GL_FALSE,glm::value_ptr(DQs[0]));

		unsigned int startIndex = st;
		glUniform1uiv(animShaderProgram->uniform("startIndex"), 1, &startIndex);
		glUniform1ui(animShaderProgram->uniform("character"), 1);

		if (shadow)
			glUniform1ui(animShaderProgram->uniform("shadow"), 1);
		else
			glUniform1ui(animShaderProgram->uniform("shadow"), 0);



	animShaderProgram->disable();
}

void BackstageWindow::createBuiltInOBJ(int BuiltInType) {
	Hierachy->createOBJ(BuiltInType);
	if (BuiltInType == 4) {
		initializeShadowMap();
		((Light*)Hierachy->activeOBJList[0])->shadow->shadowGLuint = shadowMap;
		std::cout <<"**** " << ((Light*)Hierachy->activeOBJList[0])->shadow->shadowGLuint << " shadow->shadowGLuint Setting" << std::endl;
	}
}

void BackstageWindow::SetWindowSize(int m_width, int m_height, int xPos, int yPos,int m_windowWidth,int m_windowHeight) {
	backstageWidth = m_width-4;
	backstageHeight = m_height-22;
	backstageXPos = xPos+2;
	backstageYPos = yPos+4;
	windowWidth = m_windowWidth;
	windowHeight = m_windowHeight;
	glViewport(backstageXPos, backstageYPos, backstageWidth, backstageHeight);

	//SetViewport(windowWidth, windowHeight);

	generateOutlineMap();
}

void BackstageWindow::SetViewport(int m_width, int m_height) {
	//std::cout << "SetViewport" << std::endl;
	//glViewport(backstageXPos, backstageYPos, m_width, m_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0);		//background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear up color and depth buffer

	glEnable(GL_SCISSOR_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 0);		//background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear up color and depth buffer
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_DEPTH_TEST);			//test whether an object is in front of other object?

	//glViewport(backstageXPos, backstageYPos, windowWidth, windowHeight);
	glViewport(backstageXPos, backstageYPos, backstageWidth, backstageHeight);
	glScissor(backstageXPos, backstageYPos, backstageWidth, backstageHeight);

	glDisable(GL_SCISSOR_TEST);

	
}

void BackstageWindow::setupBuffer() {
	cam = camera(glm::vec3(0.0f, 30.0f, 30.0f));
	Hierachy = new HierarchyWindow();
	grid = new Grid();
	createBuiltInOBJ(4);
	createBuiltInOBJ(1);
	createBuiltInOBJ(0);
	/*m_cylinder = new BuiltInCylinder();
	m_cube = new BuiltInCube(0);
	m_sphere = new BuiltInSphere();*/

	generateOutlineMap();
}

void BackstageWindow::generateShadowMap(glm::mat4 lightSpace,Shadow* shadow) {

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowfbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear up color and depth b

	glCullFace(GL_FRONT);
	shadowShaderProgram->use();
	for (int i = 1; i < Hierachy->objectNum; i++) {
		glUniformMatrix4fv(shadowShaderProgram->uniform("Model"), 1, GL_FALSE, glm::value_ptr(modelViewArray[i]));
		glUniformMatrix4fv(shadowShaderProgram->uniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));

		Hierachy->activeOBJList[i]->RenderPicking();
	}
	shadowShaderProgram->disable();
	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SetViewport(windowWidth, windowHeight);


}

bool BackstageWindow::initializeShadowMap() {
	glGenFramebuffers(1, &m_shadowfbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowfbo);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	//Always check that our framebuffer is ok
	if (glCheckFramebufferStatus((GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE))
		return false;

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
	
}

bool BackstageWindow::generateOutlineMap() {
	//Create the FBO
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	//Create the texture object for the primitive information buffer
	glGenTextures(1, &m_pickingTexture);
	glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
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
