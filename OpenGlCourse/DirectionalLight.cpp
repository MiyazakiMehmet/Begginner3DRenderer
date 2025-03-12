#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);

}

DirectionalLight::DirectionalLight(GLuint shadowWidth, GLuint shadowHeight,
	GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat difIntensity)
	: Light(shadowWidth, shadowHeight, red, green, blue, ambIntensity, difIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
	//For creating Light Projection we dont need perspective, instead we use orthogonal projection (For shadowing)
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);

}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
	GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	//Call base class constructor before adding new functionality
	Light::UseLight(ambientIntensityLocation, ambientColourLocation, diffuseIntensityLocation);
	//getting locating and store data
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);

}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	//Projection * view
    glm::vec3 lightPosition = glm::vec3(5.0f, 15.0f, 5.0f); // Lower height
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);

    if (glm::any(glm::isnan(lightPosition))) {
        printf("ERROR: lightPosition contains NaN!\n");
    }

    glm::mat4 lightProjection = glm::ortho(-0.8f, 0.8f, -0.8f, 0.8f, 3.0f, 50.0f);
    glm::mat4 lightView = glm::lookAt(lightPosition, target, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 lightTransform = lightProjection * lightView;

    return lightTransform;
}

DirectionalLight::~DirectionalLight()
{

}
