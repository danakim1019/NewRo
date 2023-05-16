#ifndef CYLINDER_H
#define CYLINDER_H

#include"../BasicComponent.h"
#include"../Loader.h"

class BuiltInCylinder : public OBJect {
public:
	BuiltInCylinder();
	BuiltInCylinder(float radius, float length, int rings, int slices);

	void RenderPicking();
	virtual void RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
		glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, Animation* animation) override;
	//void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
private:

	float mRadius;
	float mLength;
	int mRings;
	int mSlices;
	int mFaces;
	int mNumVerts;
	int mIndex;

	void setup();
	void generateVertexData();
	void generateEndCapVertexData(float y, int& index, int& elIndex);

	ShaderProgram* mShaderProgram;
	std::vector<float> mVertexPositions;
	std::vector<float> mVertexNormals;
	std::vector<unsigned int> mVertexIndices;
	GLuint vaoHandle;
	GLuint ssboHandle_t = 0;

	GLuint vbo_cube_vertices, vbo_cube_colors;
	GLuint ibo_cube_elements;

};

#endif