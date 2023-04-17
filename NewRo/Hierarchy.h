#ifndef _HIERARCHY_WINDOW_H_
#define _HIERARCHY_WINDOW_H_

#include"3DObjects/BuiltInCylinderObj.h"
#include"3DObjects/BuiltInSphereObj.h"
#include"3DObjects/BuiltInCubeObj.h"
#include"3DObjects/LoadedModelObj.h"

class HierarchyWindow {
public:
	HierarchyWindow();
	void createOBJ(int type);
	void drawOutline();
	void drawList(glm::mat4* modelArray, glm::mat4& view, glm::mat4& projection, glm::mat4& location, glm::vec3 camPosition, glm::vec3 lightPosition);
	unsigned int objectNum;
	std::vector<OBJect*> activeOBJList;
};


#endif