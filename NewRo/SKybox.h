#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "BasicComponent.h"

class Skybox {
private:
	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;

	ShaderProgram* spCubeMap;
	ShaderProgram* spCube;

	unsigned int loadTexture(char const* path);

	unsigned int loadCubemap(std::vector<std::string> faces);

	unsigned int cubemapTexture;

	unsigned int cubeTexture;

public:
	Skybox();
	
	void setup();
	void RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection);


};

#endif
