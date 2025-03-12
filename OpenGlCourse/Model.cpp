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

	//load Texture 
	LoadMaterials(scene);
}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++) {
		
		unsigned int materialIndex = meshToTex[i]; //grabbing material index

		//check whether index valids
		if (materialIndex < textureList.size() && textureList[materialIndex]) {
			textureList[i]->UseTexture();
		}
		//first drawed texture and draw mesh
		meshList[i]->RenderMesh();
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++) {
		if (meshList[i]) {
			delete meshList[i];
			meshList[i] = nullptr;
		}

		if (textureList[i]) {
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
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
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0]) {
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i]; //It will store each faces of model
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}

//We load the texture after we loaded the mesh 
void Model::LoadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials); //It will decide vectors size so we dont need to pushback
	printf("Model has %d materials\n", (int)scene->mNumMaterials);
	for (size_t i = 0; i < textureList.size(); i++) {
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr; //For leaking

		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			//Since you need png to load texture, you need some path
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {



				printf("Texture found: %s\n", path.C_Str());
				int idx = std::string(path.data).rfind("\\");
				std::string fileName = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("Textures/") + fileName;

				textureList[i] = new Texture(texPath.c_str());

				if (!textureList[i]->LoadTexture()) {
					printf("Failed to load texture at: %s", texPath);

					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}
		//If model does not have a diffuse texture assigned to assimp texture will not be found.
		else {
			printf("❌ No diffuse texture found for material %zu\n", i);
		}

		// If the texture is embedded in the FBX
		if (!textureList[i] && scene->mNumTextures > 0 && scene->mTextures) {
			printf("a");
			
			// Find embedded texture for this material
			for (unsigned int ti = 0; ti < scene->mNumTextures; ti++) {
				if (scene->mTextures[ti]) {
					textureList[i] = new Texture();
					if (textureList[i]->LoadMemory(scene->mTextures[ti])) {
						printf("✅ Loaded embedded texture for material %zu (Texture Index %u)\n", i, ti);
						break;  // Stop after finding the first valid texture
					}
					else {
						delete textureList[i];
						textureList[i] = nullptr;
					}
				}
			}
		}


		if (!textureList[i]) {
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->LoadTexture();
		}
	}
}

Model::~Model()
{
	ClearModel();
}