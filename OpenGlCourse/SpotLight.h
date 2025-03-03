#pragma once

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight();
	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat ambIntensity, GLfloat difIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,  //Since it is combination of directional light and point light iy have both direction and position
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edg);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat positionLocation,  GLfloat directionLocation,
		GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation,
		GLfloat edgeLocation);

	void SetFlash(glm::vec3 pos, glm::vec3 dir);


	~SpotLight();
private:
	glm::vec3 direction;

	GLfloat edge, processedEdge;

};

