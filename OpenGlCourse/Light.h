#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(GLuint shadowWidth, GLuint shadowHeight,
		  GLfloat red, GLfloat green, GLfloat blue, 
		  GLfloat ambIntensity, GLfloat difIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation);

	ShadowMap* GetShadowMap() { return shadowMap; }

	~Light();
protected:
	//Ambient
	glm::vec3 colour;
	GLfloat ambientIntensity;
	//Diffuse
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap* shadowMap;
};

