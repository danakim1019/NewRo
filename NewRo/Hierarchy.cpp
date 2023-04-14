#include"Hierarchy.h"

HierarchyWindow::HierarchyWindow() {
	objectNum = 0;
}

void HierarchyWindow::createOBJ(int type)
{
	OBJect* temp = new OBJect();
	if (type == 0) {
		temp = new BuiltInCube(0);
	}
	else if (type == 1) {
		temp = new BuiltInSphere();
	}
	else if (type == 2) {
		temp = new BuiltInCylinder();
	}

	activeOBJList.push_back(temp);
	objectNum = activeOBJList.size();
	temp->id = objectNum;

	std::cout << "add:"<<temp->name << std::endl;
	
}


void HierarchyWindow::drawList(glm::mat4* modelArray, glm::mat4& view, glm::mat4& projection, glm::mat4& location, glm::vec3 lightPosition) {
	for (int i = 0; i < objectNum; i++) {
		glm::mat4 origin = glm::mat4(1.0f);
		activeOBJList[i]->draw(modelArray[i], view, projection, origin, lightPosition);
	}
}