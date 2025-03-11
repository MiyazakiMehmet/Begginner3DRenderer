#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	diffuseIntensity = 0.0f;
}

Light::Light(GLuint shadowWidth, GLuint shadowHeight,
	         GLfloat red, GLfloat green, GLfloat blue,
	         GLfloat ambIntensity, GLfloat difIntensity)
{
	//shadow
	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);

	colour = glm::vec3(red, green, blue);
	ambientIntensity = ambIntensity;

	diffuseIntensity = difIntensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
	GLfloat diffuseIntensityLocation)
{
	//bind values with uniforms then send it to the shader
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

Light::~Light()
{

}
