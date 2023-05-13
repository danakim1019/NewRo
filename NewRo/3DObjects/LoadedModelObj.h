#ifndef _LOADED_MODEL_H
#define _LOADED_MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <sstream>
#include <vector>

#include "Mesh.h"

using namespace std;

class LoadedModelObj : public OBJect
{
public:
	/*  Model Data */
	glm::vec3 DiffuseColor;
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;
	bool hasAnimatoins = false;

	unsigned int total_vertices = 0;

	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	LoadedModelObj(const string& path, std::string sType, bool gamma=false,glm::vec3 Ka=glm::vec3(1,1,1));

	/*Bone Data*/
	unsigned int m_NumBones = 0;
	vector<VertexBoneData> Bones;
	map<string, unsigned int> Bone_Mapping;
	map<string, map<string, const aiNodeAnim*>> Animations;
	map<unsigned int, glm::vec3> skeleton_pose;
	map<string, unsigned int> Node_Mapping;
	vector<BoneInfo> m_BoneInfo;
	unsigned int NumVertices = 0;
	glm::fdualquat IdentityDQ = glm::fdualquat(glm::quat(1.f, 0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 0.f));

	/*Animations*/
	int BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms);

	vector<glm::mat4> Transforms;
	vector<glm::fdualquat> dualQuaternions;
	//vector<glm::mat2x4> DQs;
	int animationNum;

	/*Model Rendering*/
	virtual void RenderPicking() override;
	virtual void RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location, 
		glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, Animation* animation) override;

private:
	const aiScene* scene;
	Assimp::Importer importer;
	/*  Functions  */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string const& path, std::string sType);
	
	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

	/*Animations*/
	void loadBones(aiNode* node, const aiScene* scene);
	void loadMeshBones(aiMesh* mesh, vector<VertexBoneData>& VertexBoneData);
	void loadAnimations(const aiScene* scene, string BoneName, map<string, map<string, const aiNodeAnim*>>& animations);
	void ReadNodeHeirarchy(const aiScene* scene, float AnimationTime, const aiNode* pNode,
		const glm::mat4& ParentTransform, glm::vec3 startpos);

	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string& NodeName);
};

#endif