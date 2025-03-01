#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat difIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~DirectionalLight();
private:
	glm::vec3 direction; //Diffusion needs direction to adjust lightning
};

