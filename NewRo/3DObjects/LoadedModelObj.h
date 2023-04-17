#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_CTOR_INIT


#include"../BasicComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>


#include "../Loader.h"
#include "Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "assimp\scene.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"


//#include "debug.h"

#include "Interval.h"

using std::string;

class LoadedModelData
{
public:
	LoadedModelData(){}
	/*  Model Data */
	vector<Textures> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;


	string directory;


	bool gammaCorrection;
	int type;

	unsigned int total_vertices = 0;

	/*Bone Data*/
	unsigned int m_NumBones = 0;

	vector<VertexBoneData> Bones;  //per vertex, contains bond id and weights

	map<string, unsigned int> Bone_Mapping;
	map<string, map<string, const aiNodeAnim*>> Animations;

	map<unsigned int, glm::vec3> skeleton_pose;
	map<string, unsigned int> Node_Mapping;


	vector<BoneInfo> m_BoneInfo;
	unsigned int NumVertices = 0;

	glm::fdualquat IdentityDQ = glm::fdualquat(glm::quat(1.f, 0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 0.f));

	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	LoadedModelData(string const& path, bool gamma, int k);

	// draws the model, and thus all its meshes
	void Draw(int meshNum, int num);
	int BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms, vector<glm::fdualquat>& dqs);

	std::vector<Interval> m_constraints;
	void setConstraints(float x, float y);
	float getDuration();

private:
	const aiScene* scene;
	Assimp::Importer importer;
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string const& path);
	void loadBones(aiNode* node, const aiScene* scene);
	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);


	// process a mesh object
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);


	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	vector<Textures> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	void loadMeshBones(aiMesh* mesh, vector<VertexBoneData>& VertexBoneData);


	//get animation from the bone
	//populate the animation map : animation_map[animation_name][bone_name] -> animation
	void loadAnimations(const aiScene* scene, string BoneName, map<string, map<string, const aiNodeAnim*>>& animations);


	void ReadNodeHeirarchy(const aiScene* scene, float AnimationTime, const aiNode* pNode,
		const glm::mat4& ParentTransform, const glm::fdualquat& ParentDQ, glm::vec3 startpos);



	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotaion(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);



	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);


	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

};

class LoadedModelObj :public OBJect {
private:
	LoadedModelData* modelData;
	ShaderProgram* shaderProgram;

	int type;			//Shader 

	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	GLfloat shiness;

	glm::vec3 La;
	glm::vec3 Ld;
	glm::vec3 Ls;

	//glm::vec4 lightPos;
	glm::vec3 lightIntensity;

public:
	LoadedModelObj(string const& path, bool gamma, int k) {
		modelData = new LoadedModelData(path, gamma, k);

		Ka = glm::vec3(0.1, 0.1, 0.1);
		Kd = glm::vec3(0.5, 0.5, 0.5);
		Ks = glm::vec3(1, 1, 1);
		shiness = 10;

		La = glm::vec3(0.1, 0.1, 0.1);
		Ld = glm::vec3(0.5, 0.5, 0.5);
		Ls = glm::vec3(1, 1, 1);


		lightIntensity = glm::vec3(1, 1, 1);

		//name picker
		setup();

		name = "obj";
		objectType = "Loaded3D";
	}

	void setup() {

		shaderProgram = new ShaderProgram();

		shaderProgram->initFromFiles("Shader/modelTexture.vert", "Shader/modelTexture.frag");

		shaderProgram->addUniform("model");
		shaderProgram->addUniform("view");
		shaderProgram->addUniform("projection");
		shaderProgram->addUniform("NormalMatrix");

		shaderProgram->addUniform("Light.Position");
		shaderProgram->addUniform("Light.La");
		shaderProgram->addUniform("Light.Ld");
		shaderProgram->addUniform("Light.Ls");
		shaderProgram->addUniform("Material.Ka");
		shaderProgram->addUniform("Material.Kd");
		shaderProgram->addUniform("Material.Ks");
		shaderProgram->addUniform("Material.Shiness");

		shaderProgram->addUniform("ModelViewMatrix");
		shaderProgram->addUniform("viewPosition");
		shaderProgram->addUniform("location");

		shaderProgram->addUniform("texture_diffuse1");
		shaderProgram->addUniform("texture_diffuse2");
		shaderProgram->addUniform("texture_diffuse3");
		shaderProgram->addUniform("texture_diffuse4");
		shaderProgram->addUniform("texture_diffuse5");
		shaderProgram->addUniform("texture_diffuse6");
		shaderProgram->addUniform("texture_normal1");
		shaderProgram->addUniform("texture_normal2");
		shaderProgram->addUniform("texture_normal3");
		shaderProgram->addUniform("texture_normal4");
		shaderProgram->addUniform("texture_normal5");
		shaderProgram->addUniform("texture_normal6");
		shaderProgram->addUniform("texture_specular1");
		shaderProgram->addUniform("texture_specular2");
		shaderProgram->addUniform("texture_specular3");
		shaderProgram->addUniform("texture_specular4");
		shaderProgram->addUniform("texture_specular5");


	}
	void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location,glm::vec3 camPosition, glm::vec3 lightPosition) {

		glm::mat4 mview = view * model;
		glm::mat4 imvp = glm::inverse(mview);
		glm::mat3 nmat = glm::mat3(glm::transpose(imvp));

		shaderProgram->use();
		for (unsigned int i = 0; i < modelData->meshes.size(); i++) {
			glUniformMatrix4fv(shaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(shaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(shaderProgram->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));

			//glUniform4fv(shaderProgram->uniform("LightPosition"), 1, glm::value_ptr(lightPosition));

			glUniform3fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightPosition));
			glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(La));
			glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(Ld));
			glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(Ls));


			glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
			glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
			glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
			glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shiness);

			glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
			glUniformMatrix4fv(shaderProgram->uniform("location"), 1, GL_FALSE, glm::value_ptr(location));

			glUniform3fv(shaderProgram->uniform("viewPosition"), 1, glm::value_ptr(camPosition));
			//glUniform1i(shaderProgram->uniform("texture_diffuse1"), 0);

			// bind appropriate textures
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;

			for (unsigned int j = 0; j < modelData->textures_loaded.size(); j++) {
				glActiveTexture(GL_TEXTURE0 + j);

				std::stringstream ss;
				std::string number;
				std::string name = modelData->textures_loaded[j].type;

				if (name == "texture_diffuse")
					ss << diffuseNr++;
				else if (name == "texture_specular")
					ss << specularNr++;
				else if (name == "texture_normal")
					ss << normalNr++;
				else if (name == "textgure_height")
					ss << heightNr++;
				number = ss.str();

				std::string uniformName = name + number;
				glUniform1i(shaderProgram->uniform(uniformName), j);

				glBindTexture(GL_TEXTURE_2D, modelData->textures_loaded[j].id);
			}

			modelData->Draw(i, 1);
		}
		shaderProgram->disable();
	}

	void RenderPicking() {
		for (unsigned int i = 0; i < modelData->meshes.size(); i++)
			modelData->Draw(i, 1);
	}

};