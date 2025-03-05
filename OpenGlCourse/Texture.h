#pragma once

#include <GL/glew.h>

#include "stb_image.h"
#include "assimp/scene.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	//loading
	bool LoadTexture();
	bool LoadMemory(const aiTexture* texture);
	void SetupTexture(unsigned char* data, GLenum format);

	//drawing/using
	void UseTexture();
	void ClearTexture();

	~Texture();
private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};

