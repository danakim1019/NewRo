#ifndef __CUBE
#define __CUBE


#include "../Loader.h"
#include"../BasicComponent.h"


class BuiltInCube : public OBJTransform
{
public:
	BuiltInCube(int type);
	void setup();
	void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, static glm::vec4 RGB);

	int type;		//Shader ¹øÈ£

	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_colors, vbo_cube_normals, ibo_cube_elements;
	ShaderProgram* shaderProgram;

	std::vector<glm::vec3> cube_normal;

	void computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);


	std::vector<glm::vec3> cube_vertices;
	std::vector<glm::vec3> cube_colors;

};
#endif