#include"Hierarchy.h"

HierarchyWindow::HierarchyWindow() {
	objectNum = 0;
}

void HierarchyWindow::createOBJ(int type)
{
	OBJect* temp;
	if (type == 0) {
		temp = new BuiltInCube(0);
	}
	else if (type == 1) {
		temp = new BuiltInSphere();
	}
	else if (type == 2) {
		temp = new BuiltInCylinder();
	}
	else if (type == 3) {

	}

	activeOBJList.push_back(temp);

	std::cout << "add:"<<temp->name << std::endl;
	objectNum = activeOBJList.size();
}


void HierarchyWindow::drawList(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location, glm::vec3 lightPosition) {
	for (int i = 0; i < objectNum; i++) {
		if (activeOBJList[i]->name == "Cube") {
			BuiltInCube* pCube = dynamic_cast<BuiltInCube*>(activeOBJList[i]);
			pCube->draw(model, view, projection, location, lightPosition);
		}
	}
}