#include "BuiltInCubeObj.h"


BuiltInCube::BuiltInCube(int type)
{
	this->type = type;

	Ka= glm::vec3(0.5, 0.5, 0.5);
	Kd= glm::vec3(1, 0, 0);
	Ks= glm::vec3(1, 1, 1);
	shiness = 10;

	La= glm::vec3(0.9, 0.9, 0.9);
	Ld= glm::vec3(0.6,0.6,0.6);
	Ls= glm::vec3(0.6, 0.6, 0.6);

	name = "Cube";
	objectType = "Cube";
	shaderType = "Shadow";

	setup();
}

void BuiltInCube::setup()
{
	generateVertices();

	generateIndices();

	shaderProgram = new ShaderProgram();

	//create vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	if (shaderType == "Diffuse")
	{
		//load shaders
		shaderProgram->initFromFiles("Shader/Diffuse.vert", "Shader/Diffuse.frag");

		//add attributes and uniform vars
		shaderProgram->addAttribute("coord3d");
		shaderProgram->addAttribute("v_normal");

		shaderProgram->addUniform("Light.Position");
		shaderProgram->addUniform("Light.La");
		shaderProgram->addUniform("Light.Ld");
		shaderProgram->addUniform("Light.Ls");
		shaderProgram->addUniform("Material.Ka");
		shaderProgram->addUniform("Material.Kd");
		shaderProgram->addUniform("Material.Ks");
		shaderProgram->addUniform("Material.Shiness");

		shaderProgram->addUniform("ModelViewMatrix");
		shaderProgram->addUniform("NormalMatrix");
		shaderProgram->addUniform("location");

		shaderProgram->addUniform("model");
		shaderProgram->addUniform("view");
		shaderProgram->addUniform("projection");
	}
	else if (shaderType == "Shadow") {
		shaderProgram->initFromFiles("Shader/shadowNT.vert", "Shader/shadowNT.frag");

		shaderProgram->addAttribute("VertexPosition");
		shaderProgram->addAttribute("VertexNormal");

		shaderProgram->addUniform("Light.Position");
		shaderProgram->addUniform("Light.Intensity");

		shaderProgram->addUniform("isShadow");

		shaderProgram->addUniform("Material.Ka");
		shaderProgram->addUniform("Material.Kd");
		shaderProgram->addUniform("Material.Ks");
		shaderProgram->addUniform("Material.Shiness");

		shaderProgram->addUniform("ModelViewMatrix");
		shaderProgram->addUniform("NormalMatrix");
		shaderProgram->addUniform("ModelMatrix");
		shaderProgram->addUniform("MVP");

		shaderProgram->addUniform("lightSpaceMatrix");
		shaderProgram->addUniform("shadowMap");
		shaderProgram->addUniform("shadowType");

	}

	//create vbo for vertices
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexPositions.size(), vertexPositions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

	//create vbo for colors
	glGenBuffers(1, &vbo_cube_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexNormals.size(), vertexNormals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertexIndices.size(), vertexIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void BuiltInCube::RenderPicking() {
	glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, 0);
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

	float shininess = m_mat->getShiness();

	shaderProgram->use();

	//simple
	if (shaderType == "Diffuse")
	{
		glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightP));
		glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(La));
		glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(Ld));
		glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(Ls));

		glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, m_mat->getKa());
		glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, m_mat->getKd());
		glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, m_mat->getKs());
		glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shininess);

		glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		glUniformMatrix4fv(shaderProgram->uniform("location"), 1, GL_FALSE, glm::value_ptr(location));

		glUniformMatrix4fv(shaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(shaderProgram->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	}
	else if (shaderType == "Shadow") {

		glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix4fv(shaderProgram->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));

		glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, m_mat->getKa());
		glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, m_mat->getKd());
		glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, m_mat->getKs());
		glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shininess);

		glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(view * glm::vec4(lightPosition, 1.0)));
		glUniform3fv(shaderProgram->uniform("Light.Intensity"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));

		glUniformMatrix4fv(shaderProgram->uniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));

		glUniform1i(shaderProgram->uniform("shadowMap"), 0);

		glUniform1i(shaderProgram->uniform("isShadow"), shadow->isShadow);
		glUniform1i(shaderProgram->uniform("shadowType"), shadow->shadowType);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadow->shadowGLuint);
	}

	glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shaderProgram->disable();

}

void BuiltInCube::computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,bool reverse)
{
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 norm;
	v1 = glm::vec3(p1 - p2);
	v2 = glm::vec3(p1 - p3);
	//v1 = glm::normalize(v1);
	//v2 = glm::normalize(v2);
	//if (reverse) {
	//	norm = -glm::cross(v1, v2);
	//}
	//else {
	//	norm = +glm::cross(v1, v2);
	//}

	norm = glm::cross(v1, v2);
	normalize(norm);

	//vertexNormals.push_back(norm);
	//vertexNormals.push_back(norm);
	//vertexNormals.push_back(norm);

	//std::cout << norm.x<<", "<< norm.y<<", "<< norm.z << std::endl;
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

	vertexPositions.push_back(positions[0]);
	vertexPositions.push_back(positions[1]);
	vertexPositions.push_back(positions[2]);
	vertexPositions.push_back(positions[3]);

	vertexPositions.push_back(positions[4]);
	vertexPositions.push_back(positions[5]);
	vertexPositions.push_back(positions[6]);
	vertexPositions.push_back(positions[7]);

	generateNormals(positions);
}

void BuiltInCube::generateNormals(glm::vec3 positions[]) {

	for (int i = 0; i < 2; i++)
		vertexNormals.push_back(glm::vec3(1, 0, 0)); // right Side
	for (int i = 0; i < 2; i++)
		vertexNormals.push_back(glm::vec3(-1, 0, 0)); // Left Side
	for (int i = 0; i < 2; i++)
		vertexNormals.push_back(glm::vec3(0, 0, -1)); // Back Side
	for (int i = 0; i < 2; i++)
		vertexNormals.push_back(glm::vec3(0, 0, 1)); // front Side
	for (int i = 0; i < 2; i++)
		vertexNormals.push_back(glm::vec3(0, 1, 0)); // top Side
	for (int i = 0; i < 2; i++)
		vertexNormals.push_back(glm::vec3(0, -1, 0)); // Bottom Side


}

void BuiltInCube::generateIndices() {

	// front
	vertexIndices.push_back(0);	//013 123
	vertexIndices.push_back(1);
	vertexIndices.push_back(3);

	vertexIndices.push_back(2);
	vertexIndices.push_back(3);
	vertexIndices.push_back(1);
	// back 
	vertexIndices.push_back(4);  //457 567
	vertexIndices.push_back(5);
	vertexIndices.push_back(7);

	vertexIndices.push_back(6);
	vertexIndices.push_back(7);
	vertexIndices.push_back(5);
	// right
	vertexIndices.push_back(0);  //014 145
	vertexIndices.push_back(1);
	vertexIndices.push_back(4);

	vertexIndices.push_back(5);
	vertexIndices.push_back(4);
	vertexIndices.push_back(1);
	// left
	vertexIndices.push_back(3);  //237 267
	vertexIndices.push_back(2);
	vertexIndices.push_back(7);

	vertexIndices.push_back(6);
	vertexIndices.push_back(2);
	vertexIndices.push_back(7);
	// top
	vertexIndices.push_back(0);  //034 347
	vertexIndices.push_back(3);
	vertexIndices.push_back(4);

	vertexIndices.push_back(7);
	vertexIndices.push_back(4);
	vertexIndices.push_back(3);
	// bottom
	vertexIndices.push_back(1);  //125 256
	vertexIndices.push_back(2);
	vertexIndices.push_back(5);

	vertexIndices.push_back(6);
	vertexIndices.push_back(5);
	vertexIndices.push_back(2);

}