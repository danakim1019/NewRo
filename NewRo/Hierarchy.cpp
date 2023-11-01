#include"Hierarchy.h"

#define LIGHT_ID 50000

HierarchyWindow::HierarchyWindow() {
	objectNum = 0;
	//for(int i=0;i<10;i++)
	activeOBJList.push_back(new OBJect());
	objectNum = activeOBJList.size();
}

void HierarchyWindow::createOBJ(int type)
{
	OBJect* temp = new OBJect();
	if (type == 0) {
		temp = new BuiltInCube(0);
		temp->mMat = new Material();
	}
	else if (type == 1) {
		temp = new BuiltInSphere();
		temp->mMat = new Material();
	}
	else if (type == 2) {
		temp = new BuiltInCylinder();
		temp->mMat = new Material();
	}
	else if (type == 3) {		//obj load
		std::string path = "../Asset/nanosuit/nanosuit.obj";
		temp = new LoadedModelObj(path, "Shadow", true);
		temp->mMat = new Material();
	}
	else if (type == 4)		//fbx load
	{
		std::string path = "../Asset/fast.fbx";
		temp = new LoadedModelObj(path, "Shadow", true);
		temp->isAnim = true;
		temp->setScale(0.1, 0.1, 0.1);
		temp->mMat = new Material();
	}
	else if (type == 5) {		//Light
		temp = new Light(10, 10, 0);
		//Light 정보값 입력		
	}

	if (type == 5) {
		temp->mID = 1;
		activeOBJList[0] = temp;
	}
	else {
		temp->mID = objectNum + 1;
		activeOBJList.push_back(temp);
		objectNum = temp->mID;
	}

	std::cout << "add:" << temp->mName << ", id:" << temp->mID << std::endl;

}

void HierarchyWindow::drawList(glm::mat4* modelArray, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
	glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, Animation* animation) {
	for (int i = 1; i < objectNum; i++) {
		glm::mat4 origin = glm::mat4(1.0f);
		activeOBJList[i]->RenderModel(modelArray[i], view, projection, origin, camPosition, activeOBJList[0]->getPositon(), lightSpace, shadow, animation);
	}
}