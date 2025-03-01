#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat difIntensity)
	: Light(red, green, blue, ambIntensity, difIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
	GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	//Call base class constructor before adding new functionality
	Light::UseLight(ambientIntensityLocation, ambientColourLocation, diffuseIntensityLocation);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);

}

DirectionalLight::~DirectionalLight()
{

}
