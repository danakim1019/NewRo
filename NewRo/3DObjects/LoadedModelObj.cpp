#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#include "LoadedModelObj.h"
#include"meshoptimizer.h"

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

void packMesh(std::vector<PackedVertexOct>& pv, const std::vector<VertexData>& v) {
	for (size_t i = 0; i < v.size(); i++) {
		const VertexData& vi = v[i];
		PackedVertexOct& pvi = pv[i];

		pvi.px = meshopt_quantizeHalf(vi.px);
		pvi.py = meshopt_quantizeHalf(vi.py);
		pvi.pz = meshopt_quantizeHalf(vi.pz);

		float nsum = fabsf(vi.nx) + fabsf(vi.ny) + fabsf(vi.nz);
		float nx = vi.nx / nsum;
		float ny = vi.ny / nsum;
		float nz = vi.nz;

		float nu = nz >= 0 ? nx : (1 - fabsf(ny)) * (nx >= 0 ? 1 : -1);
		float nv = nz >= 0 ? ny : (1 - fabsf(nx)) * (ny >= 0 ? 1 : -1);

		pvi.nu = char(meshopt_quantizeSnorm(nu, 8));
		pvi.nv = char(meshopt_quantizeSnorm(nv, 8));

		pvi.tx = meshopt_quantizeHalf(vi.tx);
		pvi.ty = meshopt_quantizeHalf(vi.ty);

	}
}

void simplifyComplete(const MeshData& mesh, std::vector<unsigned int>lods[], std::vector<unsigned int>& indices) {
	lods[0] = mesh.indices;
	for (size_t i = 1; i < lod_count; i++) {
		std::vector<unsigned int>& lod = lods[i];

		float threshold = powf(0.4f, float(i));
		size_t target_index_count = size_t(mesh.indices.size() * threshold) / 3 * 3;
		float target_error = 1e-2f;

		const std::vector<unsigned int>& source = lods[i - 1];

		if (source.size() < target_index_count)
			target_index_count = source.size();

		lod.resize(source.size());
		size_t reduce = meshopt_simplify(&lod[0], &source[0], source.size(), &mesh.vertices[0].px, mesh.vertices.size(), sizeof(VertexData), target_index_count, target_error);

		lod.resize(reduce);
	}

	for (size_t i = 0; i < lod_count; i++) {
		std::vector<unsigned int>& lod = lods[i];

		meshopt_optimizeVertexCache(&lod[0], &lod[0], lod.size(), mesh.vertices.size());
		meshopt_optimizeOverdraw(&lod[0], &lod[0], lod.size(), &mesh.vertices[0].px, mesh.vertices.size(), sizeof(VertexData), 1.0f);
	}

	size_t total_index_count = 0;

	for (int i = 2 - 1; i >= 0; --i) {
		lod_index_offsets[i] = total_index_count;
		lod_index_counts[i] = lods[i].size();

		total_index_count += lods[i].size();
	}

	indices.resize(total_index_count);

	for (size_t i = 0; i < 2; i++) {
		memcpy(&indices[lod_index_offsets[i]], &lods[i][0], lods[i].size() * sizeof(lods[i][0]));
	}

	std::vector <VertexData> vertices = (mesh.vertices);

	meshopt_optimizeVertexFetch(&vertices[0], &indices[0], indices.size(), &vertices[0], vertices.size(), sizeof(VertexData));

	printf("%-9s: %d triangles => %d LOD levels down to %d triangle \n", "SimplifyC", int(lod_index_counts[0]) / 3, int(lod_count), int(lod_index_counts[lod_count - 1]) / 3);

	{
		const size_t kCacheSize = 16;
		meshopt_VertexCacheStatistics vcs0 = meshopt_analyzeVertexCache(&indices[lod_index_offsets[0]], lod_index_counts[0], vertices.size(), kCacheSize, 0, 0);
		meshopt_VertexFetchStatistics vfs0 = meshopt_analyzeVertexFetch(&indices[lod_index_offsets[0]], lod_index_counts[0], vertices.size(), sizeof(VertexData));
		meshopt_VertexCacheStatistics vcsN = meshopt_analyzeVertexCache(&indices[lod_index_offsets[lod_count-1]], lod_index_counts[lod_count-1], vertices.size(), kCacheSize, 0, 0);
		meshopt_VertexFetchStatistics vfsN = meshopt_analyzeVertexFetch(&indices[lod_index_offsets[lod_count-1]], lod_index_counts[lod_count-1], vertices.size(), sizeof(VertexData));

		typedef PackedVertexOct PV;

		std::vector<PV> pv(vertices.size());
		packMesh(pv, vertices);

		std::vector<unsigned char> vbuf(meshopt_encodeVertexBufferBound(vertices.size(),sizeof(PV)));
		vbuf.resize(meshopt_encodeVertexBuffer(&vbuf[0], vbuf.size(), &pv[0], vertices.size(), sizeof(PV)));

		std::vector<unsigned char> ibuf(meshopt_encodeIndexBufferBound(indices.size(), vertices.size()));
		ibuf.resize(meshopt_encodeIndexBuffer(&ibuf[0], ibuf.size(), &indices[0], indices.size()));

		printf(" % -9s ACMR %f..%f Overfetch %f...%f Codec VB %.1f bits/vertex IB %.1f bits/triangle\n",
			"",
			vcs0.acmr, vcsN.acmr, vfs0.overfetch, vfsN.overfetch,
			double(vbuf.size())/double(vertices.size())*8,
			double(ibuf.size())/double(indices.size()/3)*8);

	}

	verticesNew = vertices;

}


/*  Functions   */
// constructor, expects a filepath to a 3D model.
LoadedModelObj::LoadedModelObj(const string& path, std::string sType, bool gamma, glm::vec3 Ka) : gammaCorrection(gamma), DiffuseColor(Ka)
{
	loadModel(path, sType);
}

/*  Functions   */
// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void LoadedModelObj::loadModel(string const& path, std::string sType)
{
	// read file via ASSIMP
	//unsigned int pFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_ValidateDataStructure | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;
	//unsigned int pFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_ValidateDataStructure | aiProcess_GenNormals | aiProcess_FixInfacingNormals;
	unsigned int pFlags=aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices;
	scene = importer.ReadFile(path, pFlags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}

	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));
	name = path.substr(path.find_last_of('/') + 1, path.size());
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


int LoadedModelObj::BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms) {
	glm::mat4 Identity = glm::mat4(1.0f);
	
	float TicksPerSecond = scene->mAnimations[0]->mTicksPerSecond != 0 ?scene->mAnimations[0]->mTicksPerSecond : 25.0f;
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, scene->mAnimations[0]->mDuration);

	//
	ReadNodeHeirarchy(scene, AnimationTime, scene->mRootNode, Identity, glm::vec3(0.0f, 0.0f, 0.0f));

	Transforms.resize(m_BoneInfo.size());

	for (unsigned int i = 0; i < m_BoneInfo.size(); i++) {
		//Transforms[i] = glm::mat4(1.0f);
		Transforms[i] = m_BoneInfo[i].FinalTransformation;
	}

	return 0;

}

void LoadedModelObj::RenderPicking()
{
	for (GLuint i = 0; i < meshes.size(); i++)
		meshes[i].RenderPicking();
}

void LoadedModelObj::RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location, 
	glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, Animation* animation)
{
	for (GLuint i = 0; i < meshes.size(); i++) {
		meshes[i].shaderProgram->use();
		
		if (hasAnimatoins) {
			//animation parameter
			float TicksPersecond = scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f;
			float TimeInTicks = animation->animationTime * TicksPersecond;
			int type = BoneTransform(TimeInTicks, Transforms);

			glUniformMatrix4fv(meshes[i].shaderProgram->uniform("gBones[0]"), Transforms.size(), GL_FALSE, glm::value_ptr(Transforms[0]));
		}

		meshes[i].RenderModel(model, view, projection, location, camPosition, lightPosition, lightSpace, shadow, shaderType, m_mat);

		meshes[i].shaderProgram->disable();
	}
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
		total_vertices += mesh->mNumVertices;
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

	Bones.resize(total_vertices);

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

	////////for simplification////////
	/*MeshData data;
	for (int i = 0; i < mesh->mNumVertices; i++) {
		VertexData v;
		v.px = mesh->mVertices[i].x;
		v.py = mesh->mVertices[i].y;
		v.pz = mesh->mVertices[i].z;

		v.nx = mesh->mVertices[i].x;
		v.ny = mesh->mVertices[i].y;
		v.nz = mesh->mVertices[i].z;

		v.tx = 0;
		v.ty = 0;

		data.vertices.push_back(v);

	}
	for (int i = 0; i < mesh->mNumFaces; i++) {
		data.indices.push_back(mesh->mFaces[i].mIndices[0]);
		data.indices.push_back(mesh->mFaces[i].mIndices[1]);
		data.indices.push_back(mesh->mFaces[i].mIndices[2]);
	}

	std::vector<unsigned int> lods[5];
	std::vector<unsigned int> indicesCombined;
	simplifyComplete(data, lods, indicesCombined);*/
	

	// retreive bone information
	loadMeshBones(mesh, Bones);

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures,m_BoneInfo,Bones,shaderType,hasAnimatoins);
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
		//if Animations size > 0  this model has Animaitons
		(Animations.size() > 0) ? hasAnimatoins = true : hasAnimatoins = false;
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
	const glm::mat4& ParentTransform, glm::vec3 startpos)
{
	string NodeName(pNode->mName.data);
	const aiAnimation* pAnimation = scene->mAnimations[0];	

	glm::mat4 NodeTransformation = glm::mat4(1.0f);
	aiMatrix4x4 tp1 = pNode->mTransformation;
	NodeTransformation = glm::transpose(glm::make_mat4(&tp1.a1));

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		//Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		aiMatrix3x3 tp = RotationQ.GetMatrix();
		//convert to glm 4x4 matrix
		glm::mat4 RotationM = glm::transpose(glm::make_mat3(&tp.a1));
		RotationM[3][3] = 1;

		//Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		glm::mat4 TranslationM = glm::mat4(1.0f);
		//get the 4x4 translation matrix
		TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));

		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scale;
		CalcInterpolatedScaling(Scale, AnimationTime, pNodeAnim);
		glm::mat4 ScaleM = glm::mat4(1.0f);
		ScaleM = glm::scale(ScaleM, glm::vec3(Scale.x, Scale.y, Scale.z));
		
		//combine rotation and translation matrix
		NodeTransformation = TranslationM * RotationM* ScaleM;
	}

	//world transform in matrix
	glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

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
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(scene, AnimationTime, pNode->mChildren[i], GlobalTransformation, startpos);
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
	float DeltaTime = (float)pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
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
	//	assert(0);
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
	//assert(0);
	return 0;
}

unsigned int LoadedModelObj::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	//assert(0);
	return 0;
}

const aiNodeAnim* LoadedModelObj::FindNodeAnim(const aiAnimation* pAnimation, const string& NodeName) {
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}