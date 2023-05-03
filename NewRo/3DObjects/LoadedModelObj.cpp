#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#include "LoadedModelObj.h"

/*  Functions   */
// constructor, expects a filepath to a 3D model.
LoadedModelObj::LoadedModelObj(const string& path, std::string sType, bool gamma, glm::vec3 Ka) : gammaCorrection(gamma), DiffuseColor(Ka)
{
	loadModel(path, sType);
}

int LoadedModelObj::BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms, vector<glm::fdualquat>& dqs) {
	glm::mat4 Identity = glm::mat4(1.0f);

	//initialization
	if (scene->mNumAnimations == 0) {
		Transforms.resize(m_NumBones);
		dqs.resize(m_NumBones);
		for (unsigned int i = 0; i < m_NumBones; i++) {
			Transforms[i] = glm::mat4(1.0f);
			dqs[i] = IdentityDQ;
		}
		return 0;
	}

	unsigned int numPosKeys = scene->mAnimations[0]->mChannels[0]->mNumPositionKeys;
	float TicksPerSecond = scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f;
	float TimeInTicks = TimeInSeconds * TicksPerSecond;

	float startTime = scene->mAnimations[0]->mChannels[0]->mPositionKeys[0].mTime;
	float duration = scene->mAnimations[0]->mChannels[0]->mPositionKeys[numPosKeys - 1].mTime - startTime;
	float AnimationTime = fmod(TimeInTicks, duration);

	//
	ReadNodeHeirarchy(scene, AnimationTime, scene->mRootNode, Identity, IdentityDQ, glm::vec3(0.0f, 0.0f, 0.0f));

	Transforms.resize(m_NumBones);
	dqs.resize(m_NumBones);

	for (unsigned int i = 0; i < m_NumBones; i++) {
		Transforms[i] = glm::mat4(1.0f);
		Transforms[i] = m_BoneInfo[i].FinalTransformation;
	}

	for (unsigned int i = 0; i < dqs.size(); i++) {
		dqs[i] = IdentityDQ;
		dqs[i] = m_BoneInfo[i].FinalTransDQ;
	}

	return 0;

}

void LoadedModelObj::RenderPicking()
{
	for (GLuint i = 0; i < meshes.size(); i++)
		meshes[i].RenderPicking();
}

void LoadedModelObj::RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location, 
	glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow)
{
	for (GLuint i = 0; i < meshes.size(); i++)
		meshes[i].RenderModel(model, view, projection,location, camPosition, lightPosition,lightSpace, shadow,shaderType, m_mat);
}

/*  Functions   */
// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void LoadedModelObj::loadModel(string const& path,std::string sType)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}

	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));	
	name = path.substr(path.find_last_of('/')+1, path.size());
	std::size_t pos = name.rfind('.');
	name = name.substr(0, pos);

	objectType = "LoadedModel";
	shaderType = sType;

	//Animation load
	loadBones(scene->mRootNode, scene);
	m_BoneInfo.resize(Bone_Mapping.size());

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void LoadedModelObj::processNode(aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh LoadedModelObj::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoord = vec;
		}
		else
			vertex.TexCoord = glm::vec2(0.0f, 0.0f);

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
	loadMeshBones(mesh, Bones);

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures,shaderType);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
vector<Texture> LoadedModelObj::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	vector<Texture> textures;
	aiString path = aiString(directory);

	bool value = mat->GetTexture(aiTextureType_DIFFUSE, 0, &path, 0, 0, 0, 0, 0);
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

unsigned int LoadedModelObj::TextureFromFile(const char* path, const string& directory, bool gamma)
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

			if (Bone_Mapping.find(BoneName) == Bone_Mapping.end()) {
				BoneIndex = m_NumBones;
				m_NumBones++;
				Bone_Mapping[BoneName] = BoneIndex;
			}
		}
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		loadBones(node->mChildren[i], scene);
	}
}

void LoadedModelObj::loadMeshBones(aiMesh* mesh, vector<VertexBoneData>& VertexBoneData)
{
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		unsigned int BoneIndex = 0;
		string BoneName(mesh->mBones[i]->mName.data);

		if (Bone_Mapping.find(BoneName) != Bone_Mapping.end()) {
			BoneIndex = Bone_Mapping[BoneName];		//assign name and index

			aiMatrix4x4 tp1 = mesh->mBones[i]->mOffsetMatrix;
			m_BoneInfo[BoneIndex].offset = glm::transpose(glm::make_mat4(&tp1.a1));
		}
		else {
			printf("load MeshBones Error\n");
		}

		int nn = mesh->mBones[i]->mNumWeights;

		for (unsigned int n = 0; n < nn; n++) {
			unsigned int vid = mesh->mBones[i]->mWeights[n].mVertexId + NumVertices;		//absolute index
			float weight = mesh->mBones[i]->mWeights[n].mWeight;
			VertexBoneData[vid].AddBoneData(BoneIndex, weight);		//for a particular vertex, which bones affect it ans its weight
		}
		//from the scene, for the boneName, find the position, orientation and scale
		loadAnimations(scene, BoneName, Animations);
	}
	NumVertices += mesh->mNumVertices;
}


//get animation from the bone
//populate the animation map : animation_map[animation_name][bone_name] -> animation
void LoadedModelObj::loadAnimations(const aiScene* scene, string BoneName, map<string, map<string, const aiNodeAnim*>>& animations)
{
	for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
		const aiAnimation* pAnimation = scene->mAnimations[i];
		string animName = pAnimation->mName.data;

		bool found = false;
		for (unsigned int j = 0; j < pAnimation->mNumChannels; j++) {
			string name = pAnimation->mChannels[j]->mNodeName.data;

			if (name == BoneName) {
				animations[animName][BoneName] = pAnimation->mChannels[j];
				found = true;
				break;
			}
		}
		if (!found)
			std::cout << "Error:nofind animation for" << BoneName << std::endl;
	}
}

void LoadedModelObj::ReadNodeHeirarchy(const aiScene* scene, float AnimationTime, const aiNode* pNode,
	const glm::mat4& ParentTransform, const glm::fdualquat& parentDQ, glm::vec3 startpos)
{
	string NodeName(pNode->mName.data);
	const aiAnimation* pAnimation = scene->mAnimations[0];

	glm::mat4 NodeTransformation = glm::mat4(1.0f);
	glm::fdualquat NodeTransformationDQ = IdentityDQ;

	aiMatrix4x4 tp1 = pNode->mTransformation;
	NodeTransformation = glm::transpose(glm::make_mat4(&tp1.a1));

	const aiNodeAnim* pNodeAnim = nullptr;
	pNodeAnim = Animations[pAnimation->mName.data][NodeName];

	if (pNodeAnim) {
		//Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);

		aiMatrix3x3 tp = RotationQ.GetMatrix();
		//convert to glm 4x4 matrix
		glm::mat4 RotationM = glm::transpose(glm::make_mat3(&tp.a1));

		//Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);

		aiVector3D Scale;
		CalcInterpolatedScaling(Scale, AnimationTime, pNodeAnim);

		glm::mat4 TranslationM = glm::mat4(1.0f);
		//get the 4x4 translation matrix
		TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));

		glm::mat4 ScaleM = glm::scale(ScaleM, glm::vec3(Scale.x, Scale.y, Scale.z));

		//combine rotation and translation matrix
		NodeTransformation = TranslationM * RotationM;

		//full dual quaternion
		NodeTransformationDQ = glm::normalize(glm::fdualquat(glm::normalize(glm::quat_cast(NodeTransformation)),
			glm::vec3(NodeTransformation[3][0], NodeTransformation[3][1], NodeTransformation[3][2])));
	}

	//world transform in matrix
	glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

	//world dual quaternion
	glm::fdualquat GlobalDQ = glm::normalize(parentDQ * NodeTransformationDQ);

	unsigned int ID = 0;
	
	if (Bone_Mapping.find(NodeName) != Bone_Mapping.end()) {
		startpos.x = GlobalTransformation[3][0];
		startpos.y = GlobalTransformation[3][1];
		startpos.z = GlobalTransformation[3][2];
		ID = Bone_Mapping[NodeName];
		skeleton_pose[ID] = startpos;
	}

	if (Bone_Mapping.find(NodeName) != Bone_Mapping.end()) {
		unsigned int NodeIndex = Bone_Mapping[NodeName];

		m_BoneInfo[NodeIndex].FinalTransformation = GlobalTransformation * m_BoneInfo[NodeIndex].offset;
		glm::fdualquat offsetDQ = glm::normalize(glm::fdualquat(glm::normalize(glm::quat_cast(m_BoneInfo[NodeIndex].offset)),
			glm::vec3(m_BoneInfo[NodeIndex].offset[3][0], m_BoneInfo[NodeIndex].offset[3][1], m_BoneInfo[NodeIndex].offset[3][2])));
		m_BoneInfo[NodeIndex].FinalTransDQ = glm::normalize(GlobalDQ * offsetDQ);
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(scene, AnimationTime, pNode->mChildren[i], GlobalTransformation, GlobalDQ, startpos);
	}

}

void LoadedModelObj::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime;
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void LoadedModelObj::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	//we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)pNodeAnim->mScalingKeys[NextRotationIndex].mTime - pNodeAnim->mScalingKeys[RotationIndex].mTime;
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void LoadedModelObj::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime;
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

unsigned int LoadedModelObj::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);
	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
	return 0;
}

unsigned int LoadedModelObj::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);
	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
	return 0;
}

unsigned int LoadedModelObj::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
	return 0;
}