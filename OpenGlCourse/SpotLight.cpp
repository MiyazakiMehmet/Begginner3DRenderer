#include "SpotLight.h"

SpotLight::SpotLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
					 GLfloat ambIntensity, GLfloat difIntensity,
					 GLfloat xPos, GLfloat yPos, GLfloat zPos,
					 GLfloat xDir, GLfloat yDir, GLfloat zDir,  //Since it is combination of directional light and point light iy have both direction and position
					 GLfloat con, GLfloat lin, GLfloat exp,
					 GLfloat edg)
	: PointLight(red, green, blue, ambIntensity, difIntensity, xPos, yPos, zPos, con, lin, exp)
{
	direction = glm::vec3(xDir, yDir, zDir);
	edge = edg;
}

void SpotLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
						 GLfloat diffuseIntensityLocation, GLfloat positionLocation, GLfloat directionLocation,
						 GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation,
						 GLfloat edgeLocation)
{

}

SpotLight::~SpotLight()
{

}
