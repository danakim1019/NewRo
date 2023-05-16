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
	else if (type == 3) {		//obj loader
		temp = new LoadedModelObj("../Asset/nanosuit/nanosuit.obj","modelTexture",true);
		//temp = new LoadedModelObj("../Asset/nanosuit/nanosuit.obj", "Shadow", true);
		//temp = new LoadedModelObj("../Asset/ancientModel/source/AncientFightingIdle.fbx", "Shadow", true);
		//temp = new LoadedModelObj("../Asset/fast.fbx", "Shadow", true);
		//temp = new LoadedModelObj("../Asset/VictoryMonster.fbx", "Shadow", true);
		//temp = new LoadedModelObj("../Asset/man-business_Running.fbx", "Shadow", true);
		//temp = new LoadedModelObj("../Asset/groo.fbx", "Shadow", true); 
		//temp = new LoadedModelObj("../Asset/sponza/sponza.obj", "Shadow", true);
		//temp = new LoadedModelObj("../Asset/sponza_1.obj", "Shadow", true);
		//temp = new LoadedModelObj("../Asset/toon-cat-free/source/toon-cat.fbx", "Shadow", true);	//No Bone
		//temp = new LoadedModelObj("../Asset/chicken-free/source/Chicken-Free.fbx", "Shadow", true);
		temp->mMat = new Material();
	}
	else if (type == 4) {		//Light
		temp = new Light(10, 10, 0);
		//Light ������ �Է�		
	}

	if (type == 4) {
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

void HierarchyWindow::drawOutline() {

}

void HierarchyWindow::drawList(glm::mat4* modelArray, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
	glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, Animation* animation) {
	for (int i = 1; i < objectNum; i++) {
		glm::mat4 origin = glm::mat4(1.0f);
		activeOBJList[i]->RenderModel(modelArray[i], view, projection, origin, camPosition, activeOBJList[0]->getPositon(), lightSpace, shadow, animation);
	}
}