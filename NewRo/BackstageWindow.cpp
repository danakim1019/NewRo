#include"BackstageWindow.h"

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

BackstageWindow::BackstageWindow(int m_width, int m_height, int w_Width, int w_Height) :mBackstageWidth(m_width), mBackstageHeight(m_height), mWindowWidth(w_Width), mWindowHeight(w_Height)
{
	mFovy = 45.0f;
	mRatio = mBackstageWidth / static_cast<float>(mBackstageHeight);
	mAngle = 0;
	mLightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, -100.0f, 300.0f);

	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	mCurrentGizmoMode = ImGuizmo::LOCAL;

	setupPickingShader();
	setupBuffer();
}

BackstageWindow::~BackstageWindow() {

}

void BackstageWindow::setupPickingShader() {
	mPickingShaderProgram = new ShaderProgram();
	mPickingShaderProgram->initFromFiles("Shader/picking.vert", "Shader/picking.frag");

	mPickingShaderProgram->addUniformGroup("picking");

	mOutlineShaderProgram = new ShaderProgram();
	mOutlineShaderProgram->initFromFiles("Shader/picking.vert", "Shader/Outline.frag");

	mOutlineShaderProgram->addUniform("MVP");

	mShadowShaderProgram = new ShaderProgram();
	mShadowShaderProgram->initFromFiles("Shader/shadowMap.vert", "Shader/shadowMap.frag");

	mShadowShaderProgram->addUniformGroup("shadowMap");

	mRTShadowShaderProgram = new ShaderProgram();
	mRTShadowShaderProgram->initFromFiles("Shader/RTshadowMap.vert","Shader/RTshadowMap.frag");

	mRTShadowShaderProgram->addUniformGroup("RTshadowMap");
	for (unsigned int i = 0; i < 100; i++)
	{
		std::string name = "gBones[" + std::to_string(i) + "]";
		mRTShadowShaderProgram->addUniform(name.c_str());
	}

}

BackstageWindow::PixelInfo BackstageWindow::ReadPixel(unsigned int x, unsigned int y) {
	//Get Data from FrameBuffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);		//get object index(id) which saved first

	//Read Pixel data
	PixelInfo Pixel;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &Pixel);

	//deallocate buffer
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	return Pixel;
}

int BackstageWindow::SelectObject(int cx, int cy, int selectedObjIndex) {

	PixelInfo Pixel = ReadPixel(cx, mWindowHeight - cy - 1);
	printf("%.1f, %.1f, %.1f\n", Pixel.drawID, Pixel.objectID, Pixel.primID);

	int result;
	(Pixel.objectID != 0 && Pixel.objectID != 0.2f) ? result = Pixel.objectID : result = -1;
	return result;
}

void BackstageWindow::DrawBackstageWindow(int width, int height, int selectedObjID, float deltaTime) {

	SetViewport(width, height);

	mViewMat = mCam.GetViewMatrix();
	mProjectionMat = glm::perspective(glm::radians(mFovy), mRatio, 0.1f, 1000.0f);
	mModelMat = mModel.getMatrix();

	if (mAngle == 360)
		mAngle = 0;
	else
		mAngle += 0.01;
	float radius = 40;
	float x = glm::sin(mAngle) * radius;
	float z = glm::cos(mAngle) * radius;
	//mHierachy->activeOBJList[0]->setPosition(x, 10, z);
	mHierachy->activeOBJList[0]->setPosition(40, 10, 40);
	//mHierachy->activeOBJList[0]->setRotation(0, -x, 0);


	mHierachy->activeOBJList[2]->setPosition(0, -4.5, 0);
	mHierachy->activeOBJList[2]->setScale(100, 3, 100);

	glm::mat4 model;
	mLightView = glm::lookAt(mHierachy->activeOBJList[0]->getPositon(), glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
	mLightSpace = mLightProjection * mLightView;

	glm::mat4 origin = glm::mat4(1.0);

	pickingPhase();

	Animation* anim = new Animation();
	anim->mAnimationTime = deltaTime;

	
	if (selectedObjID > 0) {
		guizmoPhase(selectedObjID);
		outlinePhase(selectedObjID);
	}

	//draw Grid
	mGrid->draw(origin, mViewMat, mProjectionMat);

	renderPhase(((Light*)mHierachy->activeOBJList[0])->shadow, anim, deltaTime);

	generateShadowMap(mLightSpace, ((Light*)mHierachy->activeOBJList[0])->shadow, anim);

}

void BackstageWindow::pickingPhase() {
	//FBO binding
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mPickingShaderProgram->use();

		mModelViewArray = new glm::mat4[mHierachy->objectNum];
		for (int i = 0; i < mHierachy->objectNum; i++) {
			mModel.glPushMatrix();

			//Translate
			glm::vec3 position = getObject(i + 1)->getPositon();
			mModel.glTranslate(position.x, position.y, position.z);
			//Rotate
			glm::vec3 rot = getObject(i + 1)->getRotation();
			mModel.glRotate(rot.z, 0, 0, 1);
			mModel.glRotate(rot.y, 0, 1, 0);
			mModel.glRotate(rot.x, 1, 0, 0);
			//Scale
			glm::vec3 scl = getObject(i + 1)->getScale();
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), scl);
			mModel.glScale(scl.x, scl.y, scl.z);

			mModelMat = mModel.getMatrix();
			mModelViewArray[i] = mModelMat;

			glm::mat4 mMVP = mProjectionMat * mViewMat * mModelMat;

			glUniform1ui(mPickingShaderProgram->uniform("gModelIndex"), getObjectID(i + 1));
			glUniformMatrix4fv(mPickingShaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mMVP));
			mHierachy->activeOBJList[i]->RenderPicking();

			mModel.glPopMatrix();
		}

	mPickingShaderProgram->disable();


	//FBO deallocate
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void BackstageWindow::guizmoPhase(int selectedObjID) {
	ImGuizmo::SetID(selectedObjID - 1);
	glm::vec3 pos = mHierachy->activeOBJList[selectedObjID - 1]->getPositon();
	float camDistance = glm::length(pos - mCam.Position);
	glm::mat4 modelV = glm::translate(glm::mat4(1.0f), pos);

	//ImGuizmo setting
	ImGuiIO& io = ImGui::GetIO();
	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;
	static ImGuiWindowFlags gizmoWindowFlags = 0;
	ImGuizmo::SetDrawlist();

	ImGuizmo::RecomposeMatrixFromComponents((const float*)(&(mHierachy->activeOBJList[selectedObjID - 1]->mPos.x))
		, (const float*)(&(mHierachy->activeOBJList[selectedObjID - 1]->mRot.x)),
		(const float*)(&(mHierachy->activeOBJList[selectedObjID - 1]->mScale.x)),
		(float*)glm::value_ptr(mModelViewArray[selectedObjID - 1]));

	float windowWidth = (float)ImGui::GetWindowWidth();
	float windowHeight = (float)ImGui::GetWindowHeight();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
	viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
	viewManipulateTop = ImGui::GetWindowPos().y;
	gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(ImVec2(mBackstageXPos, mBackstageYPos)
		, ImVec2(mBackstageXPos + mBackstageWidth, mBackstageYPos)) ? ImGuiWindowFlags_NoMove : 0;

	glm::mat4 cameraViewM = mCam.GetViewMatrix();

	ImGuizmo::Manipulate((const float*)glm::value_ptr(cameraViewM)
		, (const float*)glm::value_ptr(mProjectionMat), mCurrentGizmoOperation, mCurrentGizmoMode, (float*)glm::value_ptr(mModelViewArray[selectedObjID - 1]), NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

	glm::vec3 Position = mCam.Position;
	glm::vec3 Rotation = glm::vec3(mCam.Yaw, mCam.Pitch, mCam.Roll);
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

	mCam.setCameraSetting(Position, Rotation, Scale);

	ImGuizmo::DecomposeMatrixToComponents((const float*)glm::value_ptr(mModelViewArray[selectedObjID - 1]),
		(float*)(&(mHierachy->activeOBJList[selectedObjID - 1]->mPos.x))
		, (float*)(&(mHierachy->activeOBJList[selectedObjID - 1]->mRot.x)),
		(float*)(&(mHierachy->activeOBJList[selectedObjID - 1]->mScale.x)));

}

void BackstageWindow::outlinePhase(int selectedObjID) {

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	float outlineScale = 1.02f;

	glm::mat4 outlineModel = mModelViewArray[selectedObjID - 1];
	outlineModel = glm::scale(outlineModel, glm::vec3(outlineScale, outlineScale, outlineScale));

	glm::mat4 mMVP = mProjectionMat * mViewMat * outlineModel;

	mOutlineShaderProgram->use();

		glUniformMatrix4fv(mPickingShaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mMVP));
		mHierachy->activeOBJList[selectedObjID - 1]->RenderPicking();

	mOutlineShaderProgram->disable();

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);

}

void BackstageWindow::generateShadowMap(glm::mat4 lightSpace, Shadow* shadow, Animation* animation) {

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowfbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);

	glCullFace(GL_FRONT);
	for (int i = 1; i < mHierachy->objectNum; i++) {
		if (!mHierachy->activeOBJList[i]->isAnim) {
			mShadowShaderProgram->use();
			glUniformMatrix4fv(mShadowShaderProgram->uniform("Model"), 1, GL_FALSE, glm::value_ptr(mModelViewArray[i]));
			glUniformMatrix4fv(mShadowShaderProgram->uniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));

			mHierachy->activeOBJList[i]->RenderPicking();
			mShadowShaderProgram->disable();
		}
		else {
			mRTShadowShaderProgram->use();
			glUniformMatrix4fv(mRTShadowShaderProgram->uniform("Model"), 1, GL_FALSE, glm::value_ptr(mModelViewArray[i]));
			glUniformMatrix4fv(mRTShadowShaderProgram->uniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));

			mHierachy->activeOBJList[i]->RenderRTShadow(mRTShadowShaderProgram);
			mRTShadowShaderProgram->disable();
		}
		
	}
	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//SetViewport(mWindowWidth, mWindowHeight);

}

void BackstageWindow::renderPhase(Shadow* shadow, Animation* animation, float deltaTime) {

	glm::mat4 origin = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadow->mShadowGLuint);

	mHierachy->drawList(mModelViewArray, mViewMat, mProjectionMat, origin, mCam.Position, glm::vec3(0, 30, 0), mLightSpace, shadow, animation);
}

void BackstageWindow::CreateBuiltInOBJ(int builtInType) {
	mHierachy->createOBJ(builtInType);
	if (builtInType == 5) {
		initializeShadowMap();
		((Light*)mHierachy->activeOBJList[0])->shadow->mShadowGLuint = mShadowMap;
	}
}

void BackstageWindow::SetWindowSize(int width, int height, int xPos, int yPos, int m_windowWidth, int m_windowHeight) {
	mBackstageWidth = width - 4;
	mBackstageHeight = height - 22;
	mBackstageXPos = xPos + 2;
	mBackstageYPos = yPos + 4;
	mWindowWidth = m_windowWidth;
	mWindowHeight = m_windowHeight;
	glViewport(mBackstageXPos, mBackstageYPos, mBackstageWidth, mBackstageHeight);

	generateOutlineMap();
}

void BackstageWindow::SetViewport(int width, int height) {

	glClearColor(0.0f, 0.0f, 0.0f, 0);		//background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear up color and depth buffer

	glEnable(GL_SCISSOR_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 0);		//background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear up color and depth buffer
	glEnable(GL_DEPTH_TEST);			//test whether an object is in front of other object?

	glViewport(mBackstageXPos, mBackstageYPos, mBackstageWidth, mBackstageHeight);
	glScissor(mBackstageXPos, mBackstageYPos, mBackstageWidth, mBackstageHeight);

	glDisable(GL_SCISSOR_TEST);

}

void BackstageWindow::setupBuffer() {
	mCam = camera(glm::vec3(0.0f, 30.0f, 30.0f));
	mHierachy = new HierarchyWindow();
	mGrid = new Grid();
	CreateBuiltInOBJ(5);
	CreateBuiltInOBJ(1);
	CreateBuiltInOBJ(0);

	generateOutlineMap();
}



bool BackstageWindow::initializeShadowMap() {
	glGenFramebuffers(1, &mShadowfbo);
	
	glGenTextures(1, &mShadowMap);
	glBindTexture(GL_TEXTURE_2D, mShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, mShadowfbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;

}

bool BackstageWindow::generateOutlineMap() {
	//Create the FBO
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	//Create the texture object for the primitive information buffer
	glGenTextures(1, &mPickingTexture);
	glBindTexture(GL_TEXTURE_2D, mPickingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, mWindowWidth, mWindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPickingTexture, 0);

	//Create the texture object for the depth buffer
	glGenTextures(1, &mDepthTexture);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mWindowWidth, mWindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);

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
