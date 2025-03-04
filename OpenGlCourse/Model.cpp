#include "Model.h"

Model::Model()
{

}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene) {
		printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
	
		return;
	}

	//this will get the very first node in the scene (the one thats on the center)
	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);
}

void Model::RenderModel()
{

}

void Model::ClearModel()
{

}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	//Scene stores the actual mesh, Node stores ID of that mesh
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	
	for (size_t i = 0; i < node->mNumChildren; i++) {
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{

}

void Model::LoadMaterials(const aiScene* scene)
{

}

Model::~Model()
{

}