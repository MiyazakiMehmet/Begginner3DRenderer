#pragma once

#include <GL/glew.h>
#include <glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, GLfloat difIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation);


	~Light();
protected:
	//Ambient
	glm::vec3 colour;
	GLfloat ambientIntensity;
	//Diffuse
	GLfloat diffuseIntensity;
};

