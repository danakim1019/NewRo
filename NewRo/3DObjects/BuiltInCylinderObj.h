// https://github.com/danakim1019/NewRo
// The MIT License(MIT)
//
//Copyright (C) 2023 by danakim1019
// 
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 

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