#pragma once

#include <gl/glew.h>

#include "Texture.h"

class Mesh {
public:
	Texture brickTexture;
	Texture dirtTexture;
	Texture plainTexture;
	
	Mesh();
	
	//Creating our Object
	void CreateMesh(GLfloat *verticies, unsigned int *indicies, unsigned int numberOfVerticies, unsigned int numberOfIndicies);
	//Rendering/Drawing
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;

	
};
