#ifndef __CUBE
#define __CUBE


#include "../Loader.h"
#include"../BasicComponent.h"


class BuiltInCube : public OBJect
{
public:
	BuiltInCube(int type);

	virtual void RenderPicking() override;
	virtual void RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
		glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, Animation* animation) override;

	int mType;		//Shader ¹øÈ£

	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_normals, ibo_cube_elements;

private:
	GLuint VAO;
	ShaderProgram* mShaderProgram;

	std::vector<glm::vec3> mVertexPositions;
	std::vector<glm::vec3> mVertexNormals;
	std::vector<unsigned int> mVertexIndices;

	void setup();

	void generateVertices();
	void generateQuadNormals(int pA,int pB,int pC,int pD);
	void generateIndices();
};
#endif