#ifndef _SPHERE_H_
#define _SPHERE_H_

#include"../BasicComponent.h"
#include "../Loader.h"
#include<vector>
//#include "stb_image.h"

class BuiltInSphere :public OBJect {

private:
	float radius;
	GLuint slices;
	GLuint stacks;

	float nVerts, elements;

	GLuint vbo_cube_vertices, vbo_cube_normals, ibo_cube_elements, vbo_cube_texture, instanceVBO;

	GLuint VAO;

	ShaderProgram* shaderProgram;

	int type;			//Shader 

	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	GLfloat shiness;

	glm::vec3 La;
	glm::vec3 Ld;
	glm::vec3 Ls;

	glm::vec4 lightPos;

	void setup();

public:

	int R;
	int G;
	int B;


	int index;

	BuiltInSphere();
	~BuiltInSphere();
	BuiltInSphere(float rad, GLuint sl, GLuint st, int type);
	void RenderPicking();
	void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location, glm::vec3 LightPos);
	void generateVerts(float* verts, float* norms, float* tex, unsigned int* el);

	int getVertexArrayHandle();

};



#endif // !_SPHERE_H_