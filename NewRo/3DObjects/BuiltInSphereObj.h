#ifndef _SPHERE_H_
#define _SPHERE_H_

#include"../BasicComponent.h"
#include "../Loader.h"
#include<vector>
//#include "stb_image.h"

class BuiltInSphere :public OBJTransform{

private:
	float radius;
	GLuint slices;
	GLuint stacks;

	float nVerts, elements;

	GLuint vbo_cube_vertices, vbo_cube_normals, ibo_cube_elements, vbo_cube_texture, instanceVBO;

	GLuint VAO;

	ShaderProgram* shaderProgram;

	int type;			//Shader Á¾·ù

public:

	int R;
	int G;
	int B;

	int index;

	BuiltInSphere();
	~BuiltInSphere();
	BuiltInSphere(float rad, GLuint sl, GLuint st, int type);
	void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::vec4 RGB, static glm::vec3 LightPos);
	void generateVerts(float* verts, float* norms, float* tex, unsigned int* el);

	int getVertexArrayHandle();

};



#endif // !_SPHERE_H_