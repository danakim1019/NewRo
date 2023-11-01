// https://github.com/danakim1019/NewRo
// The MIT License(MIT)
//
//Copyright (C) 2023 by danakim1019
// 
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 

#ifndef _LOADED_MODEL_H
#define _LOADED_MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <sstream>
#include <vector>

#include "Mesh.h"


class LoadedModelObj : public OBJect
{
public:
	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	LoadedModelObj(const std::string& path, std::string sType, bool gamma = false, glm::vec3 Ka = glm::vec3(1, 1, 1));
	/*Animations*/
	int BoneTransform(float timeInSeconds, std::vector<glm::mat4>& transforms);

	/*Model Rendering*/
	virtual void RenderPicking() override;
	virtual void RenderRTShadow(ShaderProgram* shader) override;
	virtual void RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
		glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, Animation* animation) override;

private:
	/*  Model Data */
	std::vector<Texture> mTexturesLoaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh> mMeshes;
	std::string mDirectory;
	bool bGammaCorrection;
	bool bHasAnimatoins;
	unsigned int mTotalVertices = 0;

	/*Bone Data*/
	unsigned int mNumBones = 0;
	std::vector<VertexBoneData> mBones;
	std::map<std::string, unsigned int> mBoneMapping;
	std::map<std::string, std::map<std::string, const aiNodeAnim*>> mAnimations;
	std::map<unsigned int, glm::vec3> mSkeletonPose;
	std::map<std::string, unsigned int> mNodeMapping;
	std::vector<BoneInfo> mBoneInfo;
	unsigned int mNumVertices = 0;
	glm::fdualquat mIdentityDQ = glm::fdualquat(glm::quat(1.f, 0.f, 0.f, 0.f), glm::quat(0.f, 0.f, 0.f, 0.f));

	std::vector<glm::mat4> mTransforms;
	//std::vector<glm::fdualquat> mDualQuaternions;
	int mAnimationNum;

	/*Model Load*/
	const aiScene* mScene;
	Assimp::Importer mImporter;
	/*  Functions  */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(std::string const& path, std::string sType);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false);

	/*Animations*/
	Animation* animation;
	void loadBones(aiNode* node, const aiScene* scene);
	void loadMeshBones(aiMesh* mesh, std::vector<VertexBoneData>& vertexBoneData);
	void loadAnimations(const aiScene* scene, std::string boneName, std::map<std::string, std::map<std::string, const aiNodeAnim*>>& animations);
	void readNodeHeirarchy(const aiScene* scene, float animationTime, const aiNode* pNode,
		const glm::mat4& parentTransform, glm::vec3 startPos);

	void calcInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim);
	void calcInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* pNodeAnim);
	void calcInterpolatedPosition(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim);

	unsigned int findScaling(float animationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findRotation(float animationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findPosition(float animationTime, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, const std::string& nodeName);
};

#endif