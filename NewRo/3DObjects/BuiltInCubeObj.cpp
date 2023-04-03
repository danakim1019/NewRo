#include "BuiltInCubeObj.h"


BuiltInCube::BuiltInCube(int type)
{
	this->type = type;
	setup();
}

void BuiltInCube::setup()
{

	GLfloat cube_vertices[] = {
		// front
		-1.0, -1.0, 1.0,
		1.0, -1.0, 1.0,
		1.0, 1.0, 1.0,
		-1.0, 1.0, 1.0,
		// back
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, 1.0, -1.0,
		-1.0, 1.0, -1.0,
	};

	glm::vec3 positions[8] = {
		glm::vec3(-0.5f, -0.5f, 0.5f),  //0
		glm::vec3(-0.5f, 0.5f, 0.5f), //1
		glm::vec3(0.5f, 0.5f, 0.5f), //2
		glm::vec3(0.5f, -0.5f, 0.5f), //3
		glm::vec3(-0.5f, -0.5f, -0.5f), //4
		glm::vec3(-0.5f, 0.5f, -0.5f), //5
		glm::vec3(0.5f, 0.5f, -0.5f), //6
		glm::vec3(0.5f, -0.5f, -0.5f) //7
	};

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




	shaderProgram = new ShaderProgram();

	if (type == 0)
	{

		//load shaders
		shaderProgram->initFromFiles("simple.vert", "simple.frag");

		//create vao
		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);

		//add attributes and uniform vars
		shaderProgram->addAttribute("coord3d");
		shaderProgram->addAttribute("v_color");


		shaderProgram->addUniform("mvp");

		//create vbo for vertices
		glGenBuffers(1, &vbo_cube_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 8, &cube_vertices, GL_STATIC_DRAW);
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
		glGenBuffers(1, &vbo_cube_colors);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 8, &cube_colors, GL_STATIC_DRAW);
		glVertexAttribPointer(
			shaderProgram->attribute("v_color"), // attribute
			3,                 // number of elements per vertex, here (R,G,B)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
		);
		glEnableVertexAttribArray(shaderProgram->attribute("v_color"));


		glGenBuffers(1, &ibo_cube_elements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
	}

	else if (type == 1)
	{
		//load shaders
		shaderProgram->initFromFiles("diffuse.vert", "diffuse.frag");



		//create vao
		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);

		//add attributes and uniform vars
		shaderProgram->addAttribute("coord3d");
		//shaderProgram->addAttribute("v_color");
		shaderProgram->addAttribute("v_normal");



		shaderProgram->addUniform("mvp");
		shaderProgram->addUniform("ModelViewMatrix");
		shaderProgram->addUniform("NormalMatrix");

		shaderProgram->addUniform("LightLocation");
		shaderProgram->addUniform("Kd");
		shaderProgram->addUniform("Ld");


		//create vbo for vertices
		glGenBuffers(1, &vbo_cube_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * BuiltInCube::cube_vertices.size() * 3, BuiltInCube::cube_vertices.data(), GL_STATIC_DRAW);
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
		//glGenBuffers(1, &vbo_cube_colors);
		//glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 8, &cube_colors, GL_STATIC_DRAW);
		//glVertexAttribPointer(
		//	shaderProgram->attribute("v_color"), // attribute
		//	3,                 // number of elements per vertex, here (R,G,B)
		//	GL_FLOAT,          // the type of each element
		//	GL_FALSE,          // take our values as-is
		//	0,                 // no extra data between each position
		//	0                  // offset of first element
		//);
		//glEnableVertexAttribArray(shaderProgram->attribute("v_color"));


		glGenBuffers(1, &ibo_cube_elements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

		//create vbo for colors
		glGenBuffers(1, &vbo_cube_normals);						//Color VBO 생성
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);			//버퍼를 activate("지금 이것을 다룬다")
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cube_normal.size() * 3, cube_normal.data(), GL_STATIC_DRAW);	//VBO에 데이터 저장
		glVertexAttribPointer(
			shaderProgram->attribute("v_normal"), // attribute
			3,                 // number of elements per vertex, here (R,G,B)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
		);
		glEnableVertexAttribArray(shaderProgram->attribute("v_normal"));



	}

	else if (type == 2)
	{
		//load shaders
		shaderProgram->initFromFiles("Phong.vert", "Phong.frag");

		//create vao
		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);

		//add attributes and uniform vars
		shaderProgram->addAttribute("coord3d");
		shaderProgram->addAttribute("v_normal");

		shaderProgram->addUniform("mvp");
		shaderProgram->addUniform("ModelViewMatrix");
		shaderProgram->addUniform("NormalMatrix");

		shaderProgram->addUniform("Light.Position");
		shaderProgram->addUniform("Light.La");
		shaderProgram->addUniform("Light.Ld");
		shaderProgram->addUniform("Light.Ls");

		shaderProgram->addUniform("Material.Ka");
		shaderProgram->addUniform("Material.Kd");
		shaderProgram->addUniform("Material.Ks");
		shaderProgram->addUniform("Material.Shiness");

		//create vbo for vertices
		glGenBuffers(1, &vbo_cube_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * BuiltInCube::cube_vertices.size() * 3, BuiltInCube::cube_vertices.data(), GL_STATIC_DRAW);
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
		glGenBuffers(1, &vbo_cube_normals);						//Color VBO 생성
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);			//버퍼를 activate("지금 이것을 다룬다")
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cube_normal.size() * 3, cube_normal.data(), GL_STATIC_DRAW);	//VBO에 데이터 저장
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


	}

	glBindVertexArray(0);


}

void BuiltInCube::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, static glm::vec4 RGB)
{

	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));


	glm::vec4 lightPos(50, 50, 50, 1);
	glm::vec3 kd(1, 1, 1);
	glm::vec3 ld(1, 1, 1);



	shaderProgram->use();

	//simple
	if (type == 0)
	{
		glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));


		glBindVertexArray(vaoHandle);

		int size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		float tt = size / sizeof(GLushort);
		glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	}

	//diffuse
	else if (type == 1)
	{
		glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));


		glUniform4fv(shaderProgram->uniform("LightLocation"), 1, glm::value_ptr(lightPos));
		glUniform3fv(shaderProgram->uniform("Kd"), 1, glm::value_ptr(kd));
		glUniform3fv(shaderProgram->uniform("Ld"), 1, glm::value_ptr(ld));

		glBindVertexArray(vaoHandle);

		glDrawArrays(GL_TRIANGLES, 0, cube_vertices.size() * 3);
	}

	//phong
	else if (type == 2)
	{

		glm::vec3 Ka(0.1, 0.1, 0.1);
		glm::vec3 Kd(RGB.x, RGB.y, RGB.z);
		//glm::vec3 Kd(1, 1, 0);
		glm::vec3 Ks(1, 1, 0);
		GLfloat shiness = 10;

		glm::vec3 La(0.9, 0.9, 0.9);
		glm::vec3 Ld(0.6, 0.6, 0.6);
		glm::vec3 Ls(0.6, 0.6, 0.6);

		glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

		glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightPos));
		glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(La));
		glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(Ld));
		glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(Ls));

		glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
		glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
		glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
		glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shiness);

		glBindVertexArray(vaoHandle);
		/*
				int size;
				glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
				float tt = size / sizeof(GLushort);
				glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
		*/
		glDrawArrays(GL_TRIANGLES, 0, cube_vertices.size() * 3);
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

	cube_normal.push_back(glm::cross(v1, v2));
	cube_normal.push_back(glm::cross(v1, v2));
	cube_normal.push_back(glm::cross(v1, v2));

	cube_vertices.push_back(p1);
	cube_vertices.push_back(p2);
	cube_vertices.push_back(p3);
}