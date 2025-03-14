#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f; linear = 0.0f; exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, GLfloat difIntensity, GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat con, GLfloat lin, GLfloat exp)
	: Light(1024, 1024, red, green, blue, ambIntensity, difIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con; 
	linear = lin; 
	exponent = exp;
}

void PointLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, GLfloat positionLocation,
	GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation)
{
	Light::UseLight(ambientIntensityLocation, ambientColourLocation, diffuseIntensityLocation);
	//Storying data to the uniform by accessing its location
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent); 
	  
}

PointLight::~PointLight()
{
}
