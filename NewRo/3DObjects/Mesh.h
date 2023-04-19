#pragma once

//#include "GL/glew.h"
#include"../BasicComponent.h"
#include "../Loader.h"

#include <assimp/types.h>
#include <vector>
#include <string>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



//#include "Light.h"

using namespace std;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

struct Texture
{
	unsigned int id;
	string type;
	aiString path;
};

class Mesh
{
public:
	/*  Mesh Data  */
	ShaderProgram* shaderProgram;
	vector<GLfloat> position, normal, texCoord;

	vector<GLuint> indices;
	vector<Texture> textures;
	/*  Functions  */
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, std::string shaderType);
	void Draw(glm::mat4 view, glm::mat4 model, glm::mat4 projection, glm::vec3 lightPos);
	void RenderPicking();
	void RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location, glm::vec3 camPosition, glm::vec3 lightPosition, std::string shaderType,glm::vec3 DiffuseColor);
 
private:
	/*  Render data  */
	GLuint VAO;
	/*  Functions    */
	void setupMesh(std::string shaderType);
};

//#pragma once
//
//#include"../BasicComponent.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include<glm/gtx/dual_quaternion.hpp>
//
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/quaternion.hpp>
//
//#include "../Loader.h"
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include <vector>
//
//using namespace std;
//
//#define NUM_BONES_PER_VERTEX 4
//#define ZERO_MEM(a) memset(a, 0, sizeof(a))
//#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
//#define INVALID_MATERIAL 0xFFFFFFFF
//
//struct Vertex {
//	// position
//	glm::vec3 Position;
//	// normal
//	glm::vec3 Normal;
//	// texCoords
//	glm::vec2 TexCoords;
//	// tangent
//	glm::vec3 Tangent;
//	// bitangent
//	glm::vec3 Bitangent;
//
//};
//
//struct Textures {
//	unsigned int id;
//	string type;
//	string path;
//};
//
//struct MeshEntry {
//	MeshEntry()
//	{
//		Mesh_Index = 0;
//		Num_Bones = 0;
//		BaseVertex = 0;
//		BaseIndices = 0;
//	}
//
//	unsigned int Mesh_Index;
//	unsigned int Num_Bones;
//	unsigned int BaseVertex;
//	unsigned int BaseIndices;
//};
//
//
//struct BoneInfo {
//	glm::mat4 offset;
//	glm::mat4 FinalTransformation;
//	glm::fdualquat FinalTransDQ;  //dual quaternion
//
//	BoneInfo()
//	{
//		offset = glm::mat4(1.0f);
//		FinalTransformation = glm::mat4(1.0f);
//		FinalTransDQ.real = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
//		FinalTransDQ.dual = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
//
//	}
//};
//
//struct VertexBoneData {
//	//vertex bone data
//	unsigned int BoneIDs[NUM_BONES_PER_VERTEX];
//	float Weights[NUM_BONES_PER_VERTEX];
//
//	VertexBoneData()
//	{
//		Reset();
//	};
//
//	void Reset()
//	{
//		for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; ++i)
//		{
//			BoneIDs[i] = 0;
//			Weights[i] = 0;
//		}
//	}
//
//	void AddBoneData(unsigned int BoneID, float Weight)
//	{
//		for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++) {
//			if (Weights[i] == 0.0) {
//				BoneIDs[i] = BoneID;
//				Weights[i] = Weight;
//				return;
//			}
//		}
//	}
//};
//
//class Mesh {
//public:
//	/*  Mesh Data  */
//	vector<Vertex> vertices;
//	vector<unsigned int> indices;
//
//
//	vector<Textures> textures;
//	vector<BoneInfo> bones;
//	vector<VertexBoneData> vertexBoneData;
//
//	ShaderProgram* shaderProgram;
//
//	// constructor
//	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Textures> textures, vector<BoneInfo> bones, vector<VertexBoneData> vertexBoneData) {
//		this->vertices = vertices;
//		this->indices = indices;
//		this->textures = textures;
//		this->bones = bones;
//		this->vertexBoneData = vertexBoneData;
//		// now that we have all the required data, set the vertex buffers and its attribute pointers.
//		setupMesh();
//	}
//
//	// render the mesh
//	void Draw(int num)
//	{
//		
//		glBindVertexArray(VAO);
//
//		//glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, num);
//		glDrawElements(GL_TRIANGLES, indices.size()*sizeof(GLuint), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//
//		// always good practice to set everything back to defaults once configured.
//		//glActiveTexture(GL_TEXTURE0);
//		//glBindTexture(GL_TEXTURE_2D, textures[0].id);
//	}
//
//private:
//	/*  Render data  */
//	unsigned int VAO, vertexData_vbo, EBO, vertexBones_vbo;
//	/*  Functions    */
//	// initializes all the buffer objects/arrays
//	void setupMesh()
//	{
//		shaderProgram = new ShaderProgram();
//
//		shaderProgram->initFromFiles("Shader/modelTexture.vert", "Shader/modelTexture.frag");
//
//		shaderProgram->addUniform("model");
//		shaderProgram->addUniform("view");
//		shaderProgram->addUniform("projection");
//		shaderProgram->addUniform("NormalMatrix");
//
//		shaderProgram->addUniform("Light.Position");
//		shaderProgram->addUniform("Light.La");
//		shaderProgram->addUniform("Light.Ld");
//		shaderProgram->addUniform("Light.Ls");
//		shaderProgram->addUniform("Material.Ka");
//		shaderProgram->addUniform("Material.Kd");
//		shaderProgram->addUniform("Material.Ks");
//		shaderProgram->addUniform("Material.Shiness");
//
//		shaderProgram->addUniform("ModelViewMatrix");
//		shaderProgram->addUniform("viewPosition");
//		shaderProgram->addUniform("location");
//
//		shaderProgram->addUniform("texture_diffuse1");
//		shaderProgram->addUniform("texture_diffuse2");
//		shaderProgram->addUniform("texture_normal1");
//		shaderProgram->addUniform("texture_specular1");
//
//		// create buffers/arrays
//		glGenVertexArrays(1, &VAO);
//		glGenBuffers(1, &vertexData_vbo);
//		glGenBuffers(1, &EBO);
//		glGenBuffers(1, &vertexBones_vbo);
//
//		glBindVertexArray(VAO);
//		// load data into vertex buffers
//		glBindBuffer(GL_ARRAY_BUFFER, vertexData_vbo);
//		// A great thing about structs is that their memory layout is sequential for all its items.
//		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
//		// again translates to 3/2 floats which translates to a byte array.
//		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//
//		// set the vertex attribute pointers
//		// vertex Positions
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//		// vertex normals
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//		// vertex texture coords
//		glEnableVertexAttribArray(2);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
//
//
//		glBindBuffer(GL_ARRAY_BUFFER, vertexBones_vbo);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBoneData) * vertexBoneData.size(), &vertexBoneData[0], GL_STATIC_DRAW);
//
//		glEnableVertexAttribArray(3);
//		glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);//Int values only
//
//		glEnableVertexAttribArray(4);
//		glVertexAttribPointer(4, NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, Weights));
//
//
//
//		glBindVertexArray(0);
//	}
//};