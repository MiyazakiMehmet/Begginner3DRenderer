#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat StartTurnSpeed);

	void KeyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange);

	GLfloat GetYaw() { return yaw; }
	GLfloat GetPitch() { return pitch; }


	glm::vec3 GetCameraPosition();
	glm::vec3 GetCameraDirection();
	glm::vec3 ObjectPositionAttachedToCamera(glm::vec3 offset);

	glm::mat4 CalculateViewMatrix();

	~Camera();
			
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	//Update the variables
	void update();
};

