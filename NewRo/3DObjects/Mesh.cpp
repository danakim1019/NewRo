#include "Mesh.h"

using namespace std;


Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures,
	vector<BoneInfo> bones, vector<VertexBoneData> vertexBoneData, std::string shaderType, bool hasAnimations)
{
	this->mVertices = vertices;
	this->mIndices = indices;
	this->mTextures = textures;
	this->mBones = bones;
	this->mVertexBoneData = vertexBoneData;
	this->bHasAnimations = hasAnimations;

	setupMesh(shaderType, hasAnimations);
}

void Mesh::setupMesh(std::string shaderType, bool hasAnimations)
{
	mShaderProgram = new ShaderProgram();
	if (bHasAnimations == false) {
		if (shaderType == "modelTexture") {
			mShaderProgram->initFromFiles("Shader/modelTexture.vert", "Shader/modelTexture.frag");

			//add attributes and uniform vars
			mShaderProgram->addAttribute("coord3d");
			mShaderProgram->addAttribute("v_normal");
			mShaderProgram->addAttribute("aTexCoords");

		}
		else if (shaderType == "Diffuse") {
			//load shaders
			mShaderProgram->initFromFiles("Shader/Diffuse.vert", "Shader/Diffuse.frag");

			//add attributes and uniform vars
			mShaderProgram->addAttribute("coord3d");
			mShaderProgram->addAttribute("v_normal");

		}
		else if (shaderType == "Shadow") {
			mShaderProgram->initFromFiles("Shader/shadow.vert", "Shader/shadow.frag");

			mShaderProgram->addAttribute("VertexPosition");
			mShaderProgram->addAttribute("VertexNormal");
			mShaderProgram->addAttribute("VertexColor");

		}

		mShaderProgram->addUniformGroup(shaderType);
	}
	else {
		mShaderProgram->initFromFiles("Shader/animationModel.vert", "Shader/animationModel.frag");

		mShaderProgram->addUniform("lightSpaceMatrix");

		mShaderProgram->addUniform("Light.Position");
		mShaderProgram->addUniform("Light.Intensity");
		mShaderProgram->addUniform("isShadow");
		mShaderProgram->addUniform("shadowMap");

		mShaderProgram->addUniform("hasColor");
		mShaderProgram->addUniform("shadowType");

		mShaderProgram->addUniform("ModelViewMatrix");
		mShaderProgram->addUniform("ModelMatrix");
		mShaderProgram->addUniform("MVP");
		mShaderProgram->addUniform("NormalMatrix");

		for (unsigned int i = 0; i < mBones.size(); i++) {
			std::string name = "gBones[" + std::to_string(i) + "]";
			mShaderProgram->addUniform(name.c_str());
		}
		mShaderProgram->addUniform("Material.Ka");
		mShaderProgram->addUniform("Material.Kd");
		mShaderProgram->addUniform("Material.Ks");
		mShaderProgram->addUniform("Material.Shiness");

		mShaderProgram->addUniform("texture_diffuse1");
	}


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_position);
	glGenBuffers(1, &IBO);


	glBindVertexArray(VAO);
	// vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);


	//vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const Vertex*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

	if (hasAnimations) {
		glGenBuffers(1, &vertexBones_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBones_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBoneData) * mVertexBoneData.size(), mVertexBoneData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);		//Int values only

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, Weights));
	}

	glBindVertexArray(0);
}

void Mesh::RenderPicking()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mIndices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
	glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, std::string shaderType, Material* m_mat) {
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));

	glm::vec3 objectColor(1.0f, 1.0f, 1.0f);
	float shininess = m_mat->getShiness();

	if (bHasAnimations == false) {

		if (shaderType == "modelTexture") {
			glUniformMatrix4fv(mShaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(mShaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(mShaderProgram->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

			glUniformMatrix3fv(mShaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));

			glUniform4fv(mShaderProgram->uniform("viewPosition"), 1, glm::value_ptr(view * glm::vec4(camPosition, 1.0)));

			glUniform4fv(mShaderProgram->uniform("Light.Position"), 1, glm::value_ptr(glm::vec4(lightPosition, 1.0)));
			glUniform3fv(mShaderProgram->uniform("Light.La"), 1, glm::value_ptr(objectColor));
			glUniform3fv(mShaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(objectColor));
			glUniform3fv(mShaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(objectColor));

			glUniform3fv(mShaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(objectColor));
			glUniform3fv(mShaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(objectColor));
			glUniform3fv(mShaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(objectColor));
			glUniform1fv(mShaderProgram->uniform("Material.Shiness"), 1, &shininess);

			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;

			for (unsigned int i = 0; i < mTextures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
				// retrieve texture number (the N in diffuse_textureN)
				std::stringstream ss;
				std::string number;
				std::string name = mTextures[i].type;

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
				glUniform1i(mShaderProgram->uniform(uniformName), i);

				glBindTexture(GL_TEXTURE_2D, mTextures[i].id);

			}
		}
		else if (shaderType == "Diffuse") {
			glUniform4fv(mShaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightPosition));
			glUniform3fv(mShaderProgram->uniform("Light.La"), 1, glm::value_ptr(objectColor));
			glUniform3fv(mShaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(objectColor));
			glUniform3fv(mShaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(objectColor));

			glUniform3fv(mShaderProgram->uniform("Material.Ka"), 1, m_mat->getKa());
			glUniform3fv(mShaderProgram->uniform("Material.Kd"), 1, m_mat->getKd());
			glUniform3fv(mShaderProgram->uniform("Material.Ks"), 1, m_mat->getKs());
			glUniform1fv(mShaderProgram->uniform("Material.Shiness"), 1, &shininess);

			glUniformMatrix4fv(mShaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
			glUniformMatrix3fv(mShaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
			glUniformMatrix4fv(mShaderProgram->uniform("location"), 1, GL_FALSE, glm::value_ptr(location));

			glUniformMatrix4fv(mShaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(mShaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(mShaderProgram->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		}
		else if (shaderType == "Shadow") {
			glUniformMatrix4fv(mShaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
			glUniformMatrix4fv(mShaderProgram->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(mShaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
			glUniformMatrix3fv(mShaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));

			glUniform3fv(mShaderProgram->uniform("Material.Ka"), 1, m_mat->getKa());
			glUniform3fv(mShaderProgram->uniform("Material.Kd"), 1, m_mat->getKd());
			glUniform3fv(mShaderProgram->uniform("Material.Ks"), 1, m_mat->getKs());
			glUniform1fv(mShaderProgram->uniform("Material.Shiness"), 1, &shininess);

			glUniform4fv(mShaderProgram->uniform("Light.Position"), 1, glm::value_ptr(view * glm::vec4(lightPosition, 1.0)));
			glUniform3fv(mShaderProgram->uniform("Light.Intensity"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));

			glUniformMatrix4fv(mShaderProgram->uniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));

			glUniform1i(mShaderProgram->uniform("shadowMap"), 0);

			glUniform1i(mShaderProgram->uniform("hasColor"), true);
			//glUniform1i(mShaderProgram->uniform("isShadow"), isShadowDraw);
			glUniform1i(mShaderProgram->uniform("isShadow"), shadow->bIsShadow);
			glUniform1i(mShaderProgram->uniform("shadowType"), shadow->mShadowType);


			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, shadow->mShadowGLuint);

			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;

			for (unsigned int i = 0; i < mTextures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE1 + i); // activate proper texture unit before binding
				// retrieve texture number (the N in diffuse_textureN)
				std::stringstream ss;
				std::string number;
				std::string name = mTextures[i].type;

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
				glUniform1i(mShaderProgram->uniform(uniformName), i + 1);

				glBindTexture(GL_TEXTURE_2D, mTextures[i].id);

			}
		}
	}
	else {

		glUniformMatrix4fv(mShaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix4fv(mShaderProgram->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(mShaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix3fv(mShaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));

		glUniform3fv(mShaderProgram->uniform("Material.Ka"), 1, m_mat->getKa());
		glUniform3fv(mShaderProgram->uniform("Material.Kd"), 1, m_mat->getKd());
		glUniform3fv(mShaderProgram->uniform("Material.Ks"), 1, m_mat->getKs());
		glUniform1fv(mShaderProgram->uniform("Material.Shiness"), 1, &shininess);

		glUniform4fv(mShaderProgram->uniform("Light.Position"), 1, glm::value_ptr(view * glm::vec4(lightPosition, 1.0)));
		glUniform3fv(mShaderProgram->uniform("Light.Intensity"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));

		glUniformMatrix4fv(mShaderProgram->uniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));

		glUniform1i(mShaderProgram->uniform("shadowMap"), 0);

		glUniform1i(mShaderProgram->uniform("hasColor"), false);
		glUniform1i(mShaderProgram->uniform("isShadow"), shadow->bIsShadow);
		glUniform1i(mShaderProgram->uniform("shadowType"), shadow->mShadowType);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadow->mShadowGLuint);

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < mTextures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE1 + i); // activate proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::stringstream ss;
			std::string number;
			std::string name = mTextures[i].type;

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
			glUniform1i(mShaderProgram->uniform(uniformName), i + 1);

			glBindTexture(GL_TEXTURE_2D, mTextures[i].id);

		}
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mIndices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

