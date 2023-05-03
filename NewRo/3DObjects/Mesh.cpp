#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, std::string shaderType)
{
	unsigned int posIndex = 0;
	unsigned int texIndex = 0;

	position.clear();
	normal.clear();
	texCoord.clear();

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		position.push_back(vertices[i].Position.x);
		position.push_back(vertices[i].Position.y);
		position.push_back(vertices[i].Position.z);

		normal.push_back(vertices[i].Normal.x);
		normal.push_back(vertices[i].Normal.y);
		normal.push_back(vertices[i].Normal.z);

		texCoord.push_back(vertices[i].TexCoord.x);
		texCoord.push_back(vertices[i].TexCoord.y);

		posIndex += 3;
		texIndex += 2;
	}

	this->indices = indices;
	this->textures = textures;

	setupMesh(shaderType);
}

void Mesh::setupMesh(std::string shaderType)
{
	shaderProgram = new ShaderProgram();
	if (shaderType == "modelTexture") {
		shaderProgram->initFromFiles("Shader/modelTexture.vert", "Shader/modelTexture.frag");

		//add attributes and uniform vars
		shaderProgram->addAttribute("coord3d");
		shaderProgram->addAttribute("v_normal");
		shaderProgram->addAttribute("aTexCoords");

		shaderProgram->addUniform("model");
		shaderProgram->addUniform("view");
		shaderProgram->addUniform("projection");

		shaderProgram->addUniform("NormalMatrix");
		//shaderProgram->addUniform("ModelViewMatrix");
		//shaderProgram->addUniform("location");

		shaderProgram->addUniform("viewPosition");

		shaderProgram->addUniform("Light.Position");
		shaderProgram->addUniform("Light.La");
		shaderProgram->addUniform("Light.Ld");
		shaderProgram->addUniform("Light.Ls");

		shaderProgram->addUniform("Material.Ka");
		shaderProgram->addUniform("Material.Kd");
		shaderProgram->addUniform("Material.Ks");
		shaderProgram->addUniform("Material.Shiness");

		shaderProgram->addUniform("texture_diffuse1");
		shaderProgram->addUniform("texture_diffuse2");
		shaderProgram->addUniform("texture_normal1");
		shaderProgram->addUniform("texture_specular1");
	}
	else if (shaderType == "Diffuse") {
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
		shaderProgram->initFromFiles("Shader/shadow.vert", "Shader/shadow.frag");

		shaderProgram->addAttribute("VertexPosition");
		shaderProgram->addAttribute("VertexNormal");
		shaderProgram->addAttribute("VertexColor");

		shaderProgram->addUniform("Light.Position");
		shaderProgram->addUniform("Light.Intensity");
		shaderProgram->addUniform("hasColor");
		shaderProgram->addUniform("isShadow");

		shaderProgram->addUniform("Material.Ka");
		shaderProgram->addUniform("Material.Kd");
		shaderProgram->addUniform("Material.Ks");
		shaderProgram->addUniform("Material.Shiness");

		shaderProgram->addUniform("ModelViewMatrix");
		//shaderProgram->addUniform("ProjectionMatrix");
		shaderProgram->addUniform("NormalMatrix");
		shaderProgram->addUniform("ModelMatrix");
		shaderProgram->addUniform("MVP");

		shaderProgram->addUniform("lightSpaceMatrix");
		shaderProgram->addUniform("shadowMap");
		shaderProgram->addUniform("shadowType");

		shaderProgram->addUniform("texture_diffuse1");
	}
	

	unsigned int IBO, VBO_position, VBO_normal, VBO_texture;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// vertex positions
	glGenBuffers(1, &VBO_position);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(GLfloat), position.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// vertex normals
	glGenBuffers(1, &VBO_normal);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(GLfloat), normal.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// vertex texture coords
	glGenBuffers(1, &VBO_texture);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
	glBufferData(GL_ARRAY_BUFFER, texCoord.size() * sizeof(GLfloat), texCoord.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::RenderPicking()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
	glm::vec3 camPosition, glm::vec3 lightPosition,glm::mat4& lightSpace, Shadow* shadow, std::string shaderType,Material* m_mat) {
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));
	
	glm::vec3 objectColor(1.0f, 1.0f, 1.0f);
	float shininess = m_mat->getShiness();

	shaderProgram->use();

	if (shaderType == "modelTexture") {
		glUniformMatrix4fv(shaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(shaderProgram->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		//glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mView));
		//glUniformMatrix3fv(shaderProgram->uniform("location"), 1, GL_FALSE, glm::value_ptr(location));


		glUniform4fv(shaderProgram->uniform("viewPosition"), 1, glm::value_ptr(view*glm::vec4(camPosition,1.0)));

		glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(glm::vec4(lightPosition,1.0)));
		glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(objectColor));
		glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(objectColor));
		glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(objectColor));

		glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(objectColor));
		glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(objectColor));
		glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(objectColor));
		glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shininess);
		//glUniform4fv(shaderProgram->uniform("viewPosition"), 1, glm::value_ptr(view * camPosition));\

		//glUniform4fv(shaderProgram->uniform("light.position"), 1, glm::value_ptr(view * lightPos));
		//glUniform3fv(shaderProgram->uniform("light.ambient"), 1, glm::value_ptr(light.ambient));
		//glUniform3fv(shaderProgram->uniform("light.diffuse"), 1, glm::value_ptr(light.diffuse));
		//glUniform3fv(shaderProgram->uniform("light.specular"), 1, glm::value_ptr(light.specular));

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::stringstream ss;
			std::string number;
			std::string name = textures[i].type;

			if (name == "texture_diffuse")
				ss << diffuseNr++; // transfer unsigned int to stream
			else if (name == "texture_specular")
				ss << specularNr++; // transfer unsigned int to stream
			else if (name == "texture_normal")
				ss << normalNr++; // transfer unsigned int to stream
			else if (name == "texture_height")
				ss << heightNr++; // transfer unsigned int to stream
			number = ss.str();

			std::string uniformName = name + number;
			glUniform1i(shaderProgram->uniform(uniformName), i);

			glBindTexture(GL_TEXTURE_2D, textures[i].id);

		}
	}
	else if (shaderType == "Diffuse") {
		glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightPosition));
		glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(objectColor));
		glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(objectColor));
		glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(objectColor));

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
		glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"),1,GL_FALSE,glm::value_ptr(mview));
		glUniformMatrix4fv(shaderProgram->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));

		glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, m_mat->getKa());
		glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, m_mat->getKd());
		glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, m_mat->getKs());
		glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shininess);

		glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(view*glm::vec4(lightPosition,1.0)));
		glUniform3fv(shaderProgram->uniform("Light.Intensity"), 1, glm::value_ptr(glm::vec3(1,1,1)));
		
		glUniformMatrix4fv(shaderProgram->uniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));

		glUniform1i(shaderProgram->uniform("shadowMap"),0);

		glUniform1i(shaderProgram->uniform("hasColor"), true);
		//glUniform1i(shaderProgram->uniform("isShadow"), isShadowDraw);
		glUniform1i(shaderProgram->uniform("isShadow"), shadow->isShadow);
		glUniform1i(shaderProgram->uniform("shadowType"), shadow->shadowType);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadow->shadowGLuint);

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE1 + i); // activate proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::stringstream ss;
			std::string number;
			std::string name = textures[i].type;

			if (name == "texture_diffuse")
				ss << diffuseNr++; // transfer unsigned int to stream
			else if (name == "texture_specular")
				ss << specularNr++; // transfer unsigned int to stream
			else if (name == "texture_normal")
				ss << normalNr++; // transfer unsigned int to stream
			else if (name == "texture_height")
				ss << heightNr++; // transfer unsigned int to stream
			number = ss.str();

			std::string uniformName = name + number;
			glUniform1i(shaderProgram->uniform(uniformName), i+1);

			glBindTexture(GL_TEXTURE_2D, textures[i].id);

		}
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shaderProgram->disable();
}

