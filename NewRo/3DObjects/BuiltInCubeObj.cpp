#include "BuiltInCubeObj.h"


BuiltInCube::BuiltInCube(int type)
{
	this->type = type;
	setup();
	Ka= glm::vec3(1.0, 1.0, 1.0);
	Kd= glm::vec3(0.64, 0.64, 0.64);
	Ks= glm::vec3(0.5, 0.5, 0.5);
	shiness = 10;

	La= glm::vec3(0.9, 0.9, 0.9);
	Ld= glm::vec3(0.6, 0.6, 0.6);
	Ls= glm::vec3(0.6, 0.6, 0.6);

	lightPos= glm::vec4(0, 5, 0, 1);

}

void BuiltInCube::setup()
{
	glm::vec3 positions[8] = {
		//front
		glm::vec3(1.0f, 1.0f, 0.0f),  //top right
		glm::vec3(1.0f, -1.0f, 0.0f), //bottom right
		glm::vec3(-1.0f, -1.0f, 0.0f), //bottom left
		glm::vec3(-1.0f, 1.0f, 0.0f), //top left
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


	computeNormal(positions[0], positions[1], positions[2]);
	computeNormal(positions[0], positions[2], positions[3]);

	computeNormal(positions[2], positions[3], positions[7]);
	computeNormal(positions[2], positions[7], positions[6]);

	computeNormal(positions[3], positions[0], positions[4]);
	computeNormal(positions[3], positions[4], positions[7]);

	computeNormal(positions[6], positions[5], positions[1]);
	computeNormal(positions[6], positions[1], positions[2]);

	computeNormal(positions[4], positions[5], positions[6]);
	computeNormal(positions[4], positions[6], positions[7]);

	computeNormal(positions[5], positions[4], positions[0]);
	computeNormal(positions[5], positions[0], positions[1]);


	GLfloat cube_colors[] = {
		// front colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		// back colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
	};

	
	GLushort cube_elements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,
	};

	generateIndices();

	shaderProgram = new ShaderProgram();

	if (type == 0)
	{
		//load shaders
		shaderProgram->initFromFiles("Shader/Diffuse.vert", "Shader/Diffuse.frag");

		//create vao
		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);

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


		//create vbo for vertices
		glGenBuffers(1, &vbo_cube_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexPositions.size(),vertexPositions.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(
			shaderProgram->attribute("coord3d"), // attribute
			3,                 // number of elements per vertex, here (x,y,z,1)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
		);
		glEnableVertexAttribArray(shaderProgram->attribute("coord3d"));


		//create vbo for colors
		glGenBuffers(1, &vbo_cube_normals);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexNormals.size(),vertexNormals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(
			shaderProgram->attribute("v_normal"), // attribute
			3,                 // number of elements per vertex, here (R,G,B)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
		);
		glEnableVertexAttribArray(shaderProgram->attribute("v_normal"));


		/*glGenBuffers(1, &ibo_cube_elements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);*/

		glGenBuffers(1, &ibo_cube_elements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertexIndices.size(), vertexIndices.data(), GL_STATIC_DRAW);
	}

	glBindVertexArray(0);


}

void BuiltInCube::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection,glm::mat4& location, glm::vec3 RGB)
{

	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));


	

	shaderProgram->use();

	//simple
	if (type == 0)
	{
		glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightPos));
		glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(La));
		glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(Ld));
		glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(Ls));

		glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
		glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
		glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
		glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shiness);

		glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		glUniformMatrix4fv(shaderProgram->uniform("location"), 1, GL_FALSE, glm::value_ptr(location));

		glUniformMatrix4fv(shaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(shaderProgram->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(vaoHandle);

		glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, 0);
	}


	shaderProgram->disable();

}

void BuiltInCube::computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	//glm::vec3 v1 = glm::vec3(p2 - p1);
	glm::vec3 v1 = glm::vec3(p1 - p2);
	v1 = glm::normalize(v1);
	//glm::vec3 v2 = glm::vec3(p3 - p1);
	glm::vec3 v2 = glm::vec3(p1 - p3);
	v2 = glm::normalize(v2);

	vertexNormals.push_back(glm::cross(v1, v2));
	vertexNormals.push_back(glm::cross(v1, v2));
	vertexNormals.push_back(glm::cross(v1, v2));

}

void BuiltInCube::generateIndices() {

	// front
	vertexIndices.push_back(0);	//013 123
	vertexIndices.push_back(1);
	vertexIndices.push_back(3);

	vertexIndices.push_back(1);
	vertexIndices.push_back(2);
	vertexIndices.push_back(3);
	// back 
	vertexIndices.push_back(4);  //457 567
	vertexIndices.push_back(5);
	vertexIndices.push_back(7);

	vertexIndices.push_back(5);
	vertexIndices.push_back(6);
	vertexIndices.push_back(7);
	// right
	vertexIndices.push_back(0);  //014 145
	vertexIndices.push_back(1);
	vertexIndices.push_back(4);

	vertexIndices.push_back(1);
	vertexIndices.push_back(4);
	vertexIndices.push_back(5);
	// left
	vertexIndices.push_back(2);  //237 267
	vertexIndices.push_back(3);
	vertexIndices.push_back(7);

	vertexIndices.push_back(2);
	vertexIndices.push_back(6);
	vertexIndices.push_back(7);
	// top
	vertexIndices.push_back(0);  //034 347
	vertexIndices.push_back(3);
	vertexIndices.push_back(4);

	vertexIndices.push_back(3);
	vertexIndices.push_back(4);
	vertexIndices.push_back(7);
	// bottom
	vertexIndices.push_back(1);  //125 256
	vertexIndices.push_back(2);
	vertexIndices.push_back(5);

	vertexIndices.push_back(2);
	vertexIndices.push_back(5);
	vertexIndices.push_back(6);

}