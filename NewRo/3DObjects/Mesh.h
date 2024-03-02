#pragma once

#include"../BasicComponent.h"

//#include "Light.h"

#define NUM_BONES_PER_VERTEX 4

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

struct Texture
{
	unsigned int id;
	std::string type;
	aiString path;
};

struct BoneInfo {
	glm::mat4 offset;
	glm::mat4 FinalTransformation;

	BoneInfo() {
		offset = glm::mat4(1.0f);
		FinalTransformation = glm::mat4(1.0f);
	}

	BoneInfo(aiMatrix4x4 tp1) {
		offset = glm::transpose(glm::make_mat4(&tp1.a1));
	}

};

struct VertexBoneData {
	//vertex bone data
	unsigned int BoneIDs[NUM_BONES_PER_VERTEX];
	float Weights[NUM_BONES_PER_VERTEX];

	VertexBoneData() {
		Reset();
	}

	void Reset() {
		for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++) {
			BoneIDs[i] = 0;
			Weights[i] = 0;
		}
	}

	void AddBoneData(unsigned int BoneID, float Weight) {
		for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++) {
			if (Weights[i] == 0.0)
			{
				BoneIDs[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
		}
	}
};

class Mesh
{
public:
	/*  Mesh Data  */
	ShaderProgram* mShaderProgram;
	ShaderProgram* mAnimShaderProgram;

	std::vector<Vertex> mVertices;
	std::vector<GLuint> mIndices;
	std::vector<Texture> mTextures;
	std::vector<BoneInfo> mBones;
	std::vector<VertexBoneData> mVertexBoneData;

	unsigned int IBO, VBO_normal, VBO_texture, EBO;
	unsigned int VBO_position, vertexBones_vbo;
	/*Animation Data*/
	bool bHasAnimations;

	/*  Functions  */
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures,
		std::vector<BoneInfo> bones, std::vector<VertexBoneData> vertexBoneData, std::string shaderType, bool hasAnimations);
	void Draw(glm::mat4 view, glm::mat4 model, glm::mat4 projection, glm::vec3 lightPos);
	void RenderPicking();
	void RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
		glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, std::string shaderType, Material* m_mat);

private:
	/*  Render data  */
	GLuint VAO;
	/*  Functions    */
	void setupMesh(std::string shaderType, bool hasAnimations);

};
