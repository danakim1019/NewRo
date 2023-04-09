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

	temp->id = objectNum;
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
		else if (activeOBJList[i]->name == "Sphere") {
			BuiltInSphere* pCube = dynamic_cast<BuiltInSphere*>(activeOBJList[i]);
			pCube->draw(model, view, projection, location, lightPosition);
		}
		else if (activeOBJList[i]->name == "Cylinder") {
			BuiltInCylinder* pCube = dynamic_cast<BuiltInCylinder*>(activeOBJList[i]);
			pCube->draw(model, view, projection, location,0,0,0);
		}
	}
}