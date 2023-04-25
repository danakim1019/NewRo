#ifndef __CUBE
#define __CUBE


#include "../Loader.h"
#include"../BasicComponent.h"


class BuiltInCube : public OBJect
{
private:
	GLuint VAO;
	ShaderProgram* shaderProgram;

public:
	BuiltInCube(int type);
	void setup();

	virtual void RenderPicking() override;
	virtual void RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location, glm::vec3 camPosition,glm::vec3 lightPosition) override;

	int type;		//Shader ¹øÈ£

	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_normals, ibo_cube_elements;
	

	void computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, bool reverse);
	void generateVertices();
	void generateNormals(glm::vec3 positions[]);
	void generateIndices();

	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> vertexNormals;
	std::vector<unsigned int> vertexIndices;

	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	GLfloat shiness;

	glm::vec3 La;
	glm::vec3 Ld;
	glm::vec3 Ls;

};
#endif