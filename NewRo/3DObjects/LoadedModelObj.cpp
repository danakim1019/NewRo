
#include "LoadedModelObj.h"

using namespace std;

struct VertexData {
	float px, py, pz;
	float nx, ny, nz;
	float tx, ty;
};

struct PackedVertex {
	unsigned short px, py, pz;
	unsigned short pw;
	signed char nx, ny, nz, nw;
	unsigned short tx, ty;
};

struct PackedVertexOct {
	unsigned short px, py, pz;
	signed char nu, nv;
	unsigned short tx, ty;
};

struct MeshData {
	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;
};

static const size_t lod_count = 5;

size_t lod_index_offsets[lod_count] = {};
size_t lod_index_counts[lod_count] = {};

std::vector<VertexData> verticesNew;

/*  Functions   */
// constructor, expects a filepath to a 3D model.
LoadedModelObj::LoadedModelObj(const std::string& path, std::string sType, bool gamma, glm::vec3 Ka) : bGammaCorrection(gamma)
{
	loadModel(path, sType);
}

/*  Functions   */
// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void LoadedModelObj::loadModel(std::string const& path, std::string sType)
{
	// read file via ASSIMP
	//unsigned int pFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_ValidateDataStructure | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;
	//unsigned int pFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_ValidateDataStructure | aiProcess_GenNormals | aiProcess_FixInfacingNormals;
	unsigned int pFlags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices;
	mScene = mImporter.ReadFile(path, pFlags);

	if (!mScene || mScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !mScene->mRootNode) // if is Zero
	{
		cout << "ERROR::ASSIMP:: " << mImporter.GetErrorString() << endl;
		return;
	}

	// retrieve the directory path of the filepath
	mDirectory = path.substr(0, path.find_last_of('/'));
	mName = path.substr(path.find_last_of('/') + 1, path.size());
	std::size_t pos = mName.rfind('.');
	mName = mName.substr(0, pos);

	mObjectType = "LoadedModel";
	mShaderType = sType;

	mAnimationNum = 0;

	// process ASSIMP's root node recursively
	processNode(mScene->mRootNode, mScene);
}


int LoadedModelObj::BoneTransform(float timeInSeconds, vector<glm::mat4>& transforms) {
	glm::mat4 Identity = glm::mat4(1.0f);

	float ticksPerSecond = mScene->mAnimations[mAnimationNum]->mTicksPerSecond != 0 ? mScene->mAnimations[mAnimationNum]->mTicksPerSecond : 25.0f;
	float TimeInTicks = timeInSeconds * ticksPerSecond;
	float AnimationTime = fmod(TimeInTicks, mScene->mAnimations[mAnimationNum]->mDuration);

	//
	readNodeHeirarchy(mScene, AnimationTime, mScene->mRootNode, Identity, glm::vec3(0.0f, 0.0f, 0.0f));

	transforms.resize(mBoneInfo.size());

	for (unsigned int i = 0; i < mBoneInfo.size(); i++) {
		transforms[i] = mBoneInfo[i].FinalTransformation;
	}

	return 0;

}

void LoadedModelObj::RenderPicking()
{
	for (GLuint i = 0; i < mMeshes.size(); i++) {
		mMeshes[i].RenderPicking();
	}
}

void LoadedModelObj::RenderRTShadow(ShaderProgram* shader)
{
	for (GLuint i = 0; i < mMeshes.size(); i++) {
		float TicksPersecond = mScene->mAnimations[mAnimationNum]->mTicksPerSecond != 0 ? mScene->mAnimations[mAnimationNum]->mTicksPerSecond : 25.0f;
		if (animation != NULL) {
			glUniformMatrix4fv(shader->uniform("gBones[0]"), mTransforms.size(), GL_FALSE, glm::value_ptr(mTransforms[0]));
		}
		mMeshes[i].RenderPicking();
	}
}

void LoadedModelObj::RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
	glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, Animation* animation)
{
	for (GLuint i = 0; i < mMeshes.size(); i++) {
		mMeshes[i].mShaderProgram->use();

		if (bHasAnimatoins) {
			//animation parameter
			float TicksPersecond = mScene->mAnimations[mAnimationNum]->mTicksPerSecond != 0 ? mScene->mAnimations[mAnimationNum]->mTicksPerSecond : 25.0f;
			float TimeInTicks = animation->mAnimationTime * TicksPersecond;
			int type = BoneTransform(TimeInTicks, mTransforms);

			this->animation = animation;

			glUniformMatrix4fv(mMeshes[i].mShaderProgram->uniform("gBones[0]"), mTransforms.size(), GL_FALSE, glm::value_ptr(mTransforms[0]));
		}

		mMeshes[i].RenderModel(model, view, projection, location, camPosition, lightPosition, lightSpace, shadow, mShaderType, mMat);

		mMeshes[i].mShaderProgram->disable();
	}
}


// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void LoadedModelObj::processNode(aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for (GLuint i = 0; i < scene->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[i];
		mTotalVertices += mesh->mNumVertices;
		mMeshes.push_back(processMesh(mesh, scene));
	}
}

Mesh LoadedModelObj::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	mBones.resize(mTotalVertices);

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		glm::vec3 normal;
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		if (mesh->HasNormals())
		{
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			vertex.Normal = normal;
		}

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoord = vec;
		}
		else {
			vertex.TexCoord = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	// retreive bone information
	loadMeshBones(mesh, mBones);

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures, mBoneInfo, mBones, mShaderType, bHasAnimatoins);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
vector<Texture> LoadedModelObj::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	vector<Texture> textures;
	aiString path = aiString(mDirectory);

	bool value = mat->GetTexture(aiTextureType_DIFFUSE, 0, &path, 0, 0, 0, 0, 0);
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < mTexturesLoaded.size(); j++)
		{
			if (std::strcmp(mTexturesLoaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(mTexturesLoaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = textureFromFile(str.C_Str(), this->mDirectory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			mTexturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

unsigned int LoadedModelObj::textureFromFile(const char* path, const string& directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;


	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	//	unsigned char *data = SOIL_load_image(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void LoadedModelObj::loadBones(aiNode* node, const aiScene* scene) {
	//본의 자식 갯수 만큼 처음에는 root 부터 시작
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		string NodeName(node->mChildren[i]->mName.data);
		if (NodeName.find(":") != string::npos) {
			string BoneName = NodeName;
			unsigned int BoneIndex = 0;

			if (mBoneMapping.find(BoneName) == mBoneMapping.end()) {
				BoneIndex = mNumBones;
				mNumBones++;
				mBoneMapping[BoneName] = BoneIndex;
			}
		}
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		loadBones(node->mChildren[i], scene);
	}
}

void LoadedModelObj::loadMeshBones(aiMesh* mesh, vector<VertexBoneData>& vertexBoneData)
{
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		unsigned int BoneIndex = 0;
		string BoneName(mesh->mBones[i]->mName.data);

		if (mBoneMapping.find(BoneName) == mBoneMapping.end()) {
			// Allocate an index for a new bone
			BoneIndex = (int)mBoneMapping.size();
			mBoneMapping[BoneName] = BoneIndex;
		}
		else {
			BoneIndex = mBoneMapping[BoneName];
		}

		if (BoneIndex == mBoneInfo.size()) {
			BoneInfo bi(mesh->mBones[i]->mOffsetMatrix);
			mBoneInfo.push_back(bi);
		}

		int nn = mesh->mBones[i]->mNumWeights;

		for (int n = 0; n < nn; n++) {
			unsigned int vid = mesh->mBones[i]->mWeights[n].mVertexId + mNumVertices;		//absolute index
			float weight = mesh->mBones[i]->mWeights[n].mWeight;
			vertexBoneData[vid].AddBoneData(BoneIndex, weight);		//for a particular vertex, which bones affect it ans its weight
		}
		//from the scene, for the boneName, find the position, orientation and scale
		loadAnimations(mScene, BoneName, mAnimations);
		//if Animations size > 0  this model has Animaitons
		(mScene->mNumAnimations > 0) ? bHasAnimatoins = true : bHasAnimatoins = false;
	}
	mNumVertices += mesh->mNumVertices;
}


//get animation from the bone
//populate the animation map : animation_map[animation_name][bone_name] -> animation
void LoadedModelObj::loadAnimations(const aiScene* scene, string boneName, map<string, map<string, const aiNodeAnim*>>& animations)
{
	for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
		const aiAnimation* pAnimation = scene->mAnimations[i];
		string animName = pAnimation->mName.data;

		bool found = false;
		for (unsigned int j = 0; j < pAnimation->mNumChannels; j++) {
			string name = pAnimation->mChannels[j]->mNodeName.data;

			if (name == boneName) {
				animations[animName][boneName] = pAnimation->mChannels[j];
				found = true;
				break;
			}
		}
		if (!found)
			std::cout << "Error:nofind animation for" << boneName << std::endl;
	}
}

void LoadedModelObj::readNodeHeirarchy(const aiScene* scene, float animationTime, const aiNode* pNode,
	const glm::mat4& parentTransform, glm::vec3 startpos)
{
	string NodeName(pNode->mName.data);
	const aiAnimation* pAnimation = scene->mAnimations[0];

	glm::mat4 NodeTransformation = glm::mat4(1.0f);
	aiMatrix4x4 tp1 = pNode->mTransformation;
	NodeTransformation = glm::transpose(glm::make_mat4(&tp1.a1));

	const aiNodeAnim* pNodeAnim = findNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		//Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		calcInterpolatedRotation(RotationQ, animationTime, pNodeAnim);
		aiMatrix3x3 tp = RotationQ.GetMatrix();
		//convert to glm 4x4 matrix
		glm::mat4 RotationM = glm::transpose(glm::make_mat3(&tp.a1));

		//Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		calcInterpolatedPosition(Translation, animationTime, pNodeAnim);
		glm::mat4 TranslationM = glm::mat4(1.0f);
		//get the 4x4 translation matrix
		TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));

		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scale;
		calcInterpolatedScaling(Scale, animationTime, pNodeAnim);
		glm::mat4 ScaleM = glm::mat4(1.0f);
		ScaleM = glm::scale(ScaleM, glm::vec3(Scale.x, Scale.y, Scale.z));

		//combine rotation and translation matrix
		NodeTransformation = TranslationM * RotationM * ScaleM;
	}

	//world transform in matrix
	glm::mat4 GlobalTransformation = parentTransform * NodeTransformation;

	unsigned int ID = 0;

	if (mBoneMapping.find(NodeName) != mBoneMapping.end()) {
		startpos.x = GlobalTransformation[3][0];
		startpos.y = GlobalTransformation[3][1];
		startpos.z = GlobalTransformation[3][2];
		ID = mBoneMapping[NodeName];
		mSkeletonPose[ID] = startpos;
	}

	if (mBoneMapping.find(NodeName) != mBoneMapping.end()) {
		unsigned int NodeIndex = mBoneMapping[NodeName];

		mBoneInfo[NodeIndex].FinalTransformation = GlobalTransformation * mBoneInfo[NodeIndex].offset;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
		readNodeHeirarchy(scene, animationTime, pNode->mChildren[i], GlobalTransformation, startpos);
	}
}

void LoadedModelObj::calcInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = findScaling(animationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime;
	float Factor = (animationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	out = Start + Factor * Delta;
}

void LoadedModelObj::calcInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* pNodeAnim)
{
	//we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = findRotation(animationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
	float Factor = (animationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(out, StartRotationQ, EndRotationQ, Factor);
	out = out.Normalize();
}

void LoadedModelObj::calcInterpolatedPosition(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = findPosition(animationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime;
	float Factor = (animationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	out = Start + Factor * Delta;
}

unsigned int LoadedModelObj::findScaling(float animationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);
	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (animationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}
	//	assert(0);
	return 0;
}

unsigned int LoadedModelObj::findRotation(float animationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);
	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (animationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}
	//assert(0);
	return 0;
}

unsigned int LoadedModelObj::findPosition(float animationTime, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (animationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	//assert(0);
	return 0;
}

const aiNodeAnim* LoadedModelObj::findNodeAnim(const aiAnimation* pAnimation, const string& NodeName) {
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}