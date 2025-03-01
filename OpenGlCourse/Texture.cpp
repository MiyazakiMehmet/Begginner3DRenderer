#include "Texture.h"

#include <GL/glew.h>

#include "stb_image.h"
#include <iostream>

Texture::Texture()
{
	textureID = 0;
	width = 0, height = 0, bitDepth = 0;
	fileLocation = "";
}

Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0, height = 0, bitDepth = 0;
	fileLocation = fileLoc;
}

void Texture::LoadTexture()
{
	stbi_set_flip_vertically_on_load(true); // Flip before loading

	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	if (!texData) {
		printf("Failed to find: %s\n", fileLocation);

		return;
	}
	else {
		std::cout << "Loaded image with " << bitDepth << " channels" << std::endl;

		if (bitDepth == 1 || bitDepth == 2) {  // Possible indexed color
			std::cerr << "Warning: Image might be in indexed color mode!" << std::endl;
		}
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//S -> x axis, T -> y axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//loading with datas
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	//frees data
	stbi_image_free(texData);

}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::~Texture()
{
	ClearTexture();
}
