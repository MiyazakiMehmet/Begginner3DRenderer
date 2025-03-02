#include "Mesh.h"
#include <iostream>

Mesh::Mesh() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* verticies, unsigned int* indicies, unsigned int numberOfVerticies, unsigned int numberOfIndicies)
{
	indexCount = numberOfIndicies;

	//Vertex Array
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Index Buffer
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies[0]) * numberOfIndicies, indicies, GL_STATIC_DRAW);


	//Vertex Buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies[0]) * numberOfVerticies, verticies, GL_STATIC_DRAW);	//Stores data we want to store

	//These are the layouts
	//for object (it stores all necessary data from vertecies)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(verticies[0]) * 8, 0);//stride will read verticaly and when 3 slots get full it wont iterate for 4th
	glEnableVertexAttribArray(0);
	//for texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(verticies[0]) * 8, (void*)(sizeof(verticies[0]) * 3));//we need to make last parameter void* in attribpointer 
	glEnableVertexAttribArray(1);
	//for normal(direction)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(verticies[0]) * 8, (void*)(sizeof(verticies[0]) * 5));//we need to make last parameter void* in attribpointer 
	glEnableVertexAttribArray(2);

	//|A| = magnitude, when normalized it is = 1 


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTexture();

}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	indexCount = 0;
}

Mesh::~Mesh() {
	ClearMesh();
}