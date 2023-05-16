#ifndef _SPHERE_H_
#define _SPHERE_H_

#include"../BasicComponent.h"
#include "../Loader.h"
#include<vector>
//#include "stb_image.h"

class BuiltInSphere :public OBJect {

public:

	int mIndex;

	BuiltInSphere();
	~BuiltInSphere();
	BuiltInSphere(float rad, GLuint sl, GLuint st, int type);
	virtual void RenderPicking() override;
	virtual void RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
		glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, Animation* animation) override;

	int getVertexArrayHandle();

private:
	float mRadius;
	GLuint mSlices;
	GLuint mStacks;

	float mNumVerts, mElements;

	GLuint VAO;
	GLuint vbo_cube_vertices;
	GLuint vbo_cube_normals;
	GLuint ibo_cube_elements;
	GLuint vbo_cube_texture;
	GLuint instanceVBO;


	ShaderProgram* mShaderProgram;

	void setup();
	void generateVerts(float* verts, float* norms, float* tex, unsigned int* el);
};



#endif // !_SPHERE_H_