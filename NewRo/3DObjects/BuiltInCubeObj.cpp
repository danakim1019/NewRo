#include "BuiltInCubeObj.h"


BuiltInCube::BuiltInCube(int type)
{
	this->type = type;

	mName = "Cube";
	mObjectType = "Cube";
	mShaderType = "Shadow";

	setup();
}

void BuiltInCube::setup()
{
	generateVertices();
	generateIndices();

	mShaderProgram = new ShaderProgram();

	if (mShaderType == "Diffuse")
	{
		//load shaders
		mShaderProgram->initFromFiles("Shader/Diffuse.vert", "Shader/Diffuse.frag");
	}
	else if (mShaderType == "Shadow") {
		mShaderProgram->initFromFiles("Shader/shadow.vert", "Shader/shadow.frag");
	}

	mShaderProgram->addAttribute("VertexPosition");
	mShaderProgram->addAttribute("VertexNormal");

	mShaderProgram->addUniformGroup(mShaderType);


	//create vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);
	//create vbo for vertices
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mVertexPositions.size(), mVertexPositions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

	//create vbo for colors
	glGenBuffers(1, &vbo_cube_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mVertexNormals.size(), mVertexNormals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mVertexIndices.size(), mVertexIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void BuiltInCube::RenderPicking() {
	glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES, mVertexIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void BuiltInCube::RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
	glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, Animation* animation)
{
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));

	glm::vec4 lightP = glm::vec4(lightPosition, 1.0f);
	glm::vec3 objectColor(1.0f, 1.0f, 1.0f);

	float shininess = mMat->getShiness();

	mShaderProgram->use();

	//simple
	if (mShaderType == "Diffuse")
	{
		glUniform4fv(mShaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightP));
		glUniform3fv(mShaderProgram->uniform("Light.La"), 1, glm::value_ptr(objectColor));
		glUniform3fv(mShaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(objectColor));
		glUniform3fv(mShaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(objectColor));

		glUniform3fv(mShaderProgram->uniform("Material.Ka"), 1, mMat->getKa());
		glUniform3fv(mShaderProgram->uniform("Material.Kd"), 1, mMat->getKd());
		glUniform3fv(mShaderProgram->uniform("Material.Ks"), 1, mMat->getKs());
		glUniform1fv(mShaderProgram->uniform("Material.Shiness"), 1, &shininess);

		glUniformMatrix4fv(mShaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix3fv(mShaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		glUniformMatrix4fv(mShaderProgram->uniform("location"), 1, GL_FALSE, glm::value_ptr(location));

		glUniformMatrix4fv(mShaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(mShaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(mShaderProgram->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	}
	else if (mShaderType == "Shadow") {

		glUniformMatrix4fv(mShaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix4fv(mShaderProgram->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(mShaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix3fv(mShaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));

		glUniform3fv(mShaderProgram->uniform("Material.Ka"), 1, mMat->getKa());
		glUniform3fv(mShaderProgram->uniform("Material.Kd"), 1, mMat->getKd());
		glUniform3fv(mShaderProgram->uniform("Material.Ks"), 1, mMat->getKs());
		glUniform1fv(mShaderProgram->uniform("Material.Shiness"), 1, &shininess);

		glUniform4fv(mShaderProgram->uniform("Light.Position"), 1, glm::value_ptr(view * glm::vec4(lightPosition, 1.0)));
		glUniform3fv(mShaderProgram->uniform("Light.Intensity"), 1, glm::value_ptr(objectColor));

		glUniformMatrix4fv(mShaderProgram->uniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));

		glUniform1i(mShaderProgram->uniform("shadowMap"), 0);

		glUniform1i(mShaderProgram->uniform("isShadow"), shadow->bIsShadow);
		glUniform1i(mShaderProgram->uniform("shadowType"), shadow->mShadowType);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadow->mShadowGLuint);
	}

	glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES, mVertexIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	mShaderProgram->disable();

}

void BuiltInCube::generateVertices() {
	glm::vec3 positions[8] = {
		//front
		glm::vec3(1.0f, 1.0f, 1.0f),  //top right
		glm::vec3(1.0f, -1.0f, 1.0f), //bottom right
		glm::vec3(-1.0f, -1.0f, 1.0f), //bottom left
		glm::vec3(-1.0f, 1.0f, 1.0f), //top left
		//back
		glm::vec3(1.0f, 1.0f, -1.0f), //top right
		glm::vec3(1.0f, -1.0f, -1.0f), //bottom right
		glm::vec3(-1.0f, -1.0f, -1.0f), //bottom left
		glm::vec3(-1.0f, 1.0f, -1.0f) //top left
	};

	mVertexPositions.push_back(positions[0]);
	mVertexPositions.push_back(positions[1]);
	mVertexPositions.push_back(positions[2]);
	mVertexPositions.push_back(positions[3]);

	mVertexPositions.push_back(positions[4]);
	mVertexPositions.push_back(positions[5]);
	mVertexPositions.push_back(positions[6]);
	mVertexPositions.push_back(positions[7]);

	generateNormals(positions);
}

void BuiltInCube::generateNormals(glm::vec3 positions[]) {

	for (int i = 0; i < 2; i++)
		mVertexNormals.push_back(glm::vec3(1, 0, 0)); // right Side
	for (int i = 0; i < 2; i++)
		mVertexNormals.push_back(glm::vec3(-1, 0, 0)); // Left Side
	for (int i = 0; i < 2; i++)
		mVertexNormals.push_back(glm::vec3(0, 0, -1)); // Back Side
	for (int i = 0; i < 2; i++)
		mVertexNormals.push_back(glm::vec3(0, 0, 1)); // front Side
	for (int i = 0; i < 2; i++)
		mVertexNormals.push_back(glm::vec3(0, 1, 0)); // top Side
	for (int i = 0; i < 2; i++)
		mVertexNormals.push_back(glm::vec3(0, -1, 0)); // Bottom Side


}

void BuiltInCube::generateIndices() {

	// front
	mVertexIndices.push_back(0);	//013 123
	mVertexIndices.push_back(1);
	mVertexIndices.push_back(3);

	mVertexIndices.push_back(2);
	mVertexIndices.push_back(3);
	mVertexIndices.push_back(1);
	// back 
	mVertexIndices.push_back(4);  //457 567
	mVertexIndices.push_back(5);
	mVertexIndices.push_back(7);

	mVertexIndices.push_back(6);
	mVertexIndices.push_back(7);
	mVertexIndices.push_back(5);
	// right
	mVertexIndices.push_back(0);  //014 145
	mVertexIndices.push_back(1);
	mVertexIndices.push_back(4);

	mVertexIndices.push_back(5);
	mVertexIndices.push_back(4);
	mVertexIndices.push_back(1);
	// left
	mVertexIndices.push_back(3);  //237 267
	mVertexIndices.push_back(2);
	mVertexIndices.push_back(7);

	mVertexIndices.push_back(6);
	mVertexIndices.push_back(2);
	mVertexIndices.push_back(7);
	// top
	mVertexIndices.push_back(0);  //034 347
	mVertexIndices.push_back(3);
	mVertexIndices.push_back(4);

	mVertexIndices.push_back(7);
	mVertexIndices.push_back(4);
	mVertexIndices.push_back(3);
	// bottom
	mVertexIndices.push_back(1);  //125 256
	mVertexIndices.push_back(2);
	mVertexIndices.push_back(5);

	mVertexIndices.push_back(6);
	mVertexIndices.push_back(5);
	mVertexIndices.push_back(2);

}