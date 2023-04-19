#ifndef _LOADED_MODEL_H
#define _LOADED_MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <string>
#include <sstream>
#include <vector>



#include "Mesh.h"
//#include "clickableObject.h"


using namespace std;

class LoadedModelObj : public OBJect
{
public:
	glm::vec3 DiffuseColor;
	/*  Model Data */
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;

	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	LoadedModelObj(const string& path, std::string sType, bool gamma=false,glm::vec3 Ka=glm::vec3(1,1,1));
	virtual void RenderPicking() override;
	virtual void RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location, glm::vec3 camPosition, glm::vec3 lightPosition) override;

private:
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
};

#endif