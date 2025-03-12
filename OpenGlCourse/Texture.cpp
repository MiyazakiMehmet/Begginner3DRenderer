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

// Loads texture from file
bool Texture::LoadTexture() {
	stbi_set_flip_vertically_on_load(true);
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	if (!texData) {
		printf("❌ Failed to find: %s\n", fileLocation);
		return false;
	}

	GLenum format = (bitDepth == 4) ? GL_RGBA : GL_RGB;
	SetupTexture(texData, format);
	stbi_image_free(texData);

	return true;
}

// Loads texture from memory (for embedded textures)
bool Texture::LoadMemory(const aiTexture* texture) {
	if (!texture) return false;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* image_data = nullptr;

	if (texture->mHeight == 0) {
		// Compressed format (PNG, JPG, etc.)
		image_data = stbi_load_from_memory(
			reinterpret_cast<unsigned char*>(texture->pcData),
			texture->mWidth, &width, &height, &bitDepth, 0
		);
	}
	else {
		// Raw pixel data
		image_data = stbi_load_from_memory(
			reinterpret_cast<unsigned char*>(texture->pcData),
			texture->mWidth * texture->mHeight, &width, &height, &bitDepth, 0
		);
	}

	if (!image_data) {
		std::cerr << "❌ Failed to load embedded texture from memory!" << std::endl;
		return false;
	}

	GLenum format = (bitDepth == 4) ? GL_RGBA : GL_RGB;
	SetupTexture(image_data, format);
	stbi_image_free(image_data);

	std::cout << "✅ Loaded embedded texture with " << bitDepth << " channels" << std::endl;
	return true;
}

// Sets up OpenGL texture
void Texture::SetupTexture(unsigned char* data, GLenum format) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
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

