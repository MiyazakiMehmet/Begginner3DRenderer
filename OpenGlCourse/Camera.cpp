﻿#include "Camera.h"

Camera::Camera()
{

}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat StartTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = startMoveSpeed;
	turnSpeed = StartTurnSpeed;

	update();
}

void Camera::KeyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;


	if (keys[GLFW_KEY_W]) {
		position += front * velocity;
	}
	if (keys[GLFW_KEY_D]) {
		position += right * velocity;
	}
	if (keys[GLFW_KEY_A]) {
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_S]) {
		position -= front * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}


	//Since we changed our pitch and yaw we should update our front, right and up
	update();
}

glm::vec3 Camera::GetCameraPosition()
{
	return position;
}

glm::vec3 Camera::GetCameraDirection()
{
	return glm::normalize(front);
}

glm::vec3 Camera::ObjectPositionAttachedToCamera(glm::vec3 offset)
{

	// Step 1: Rotate around Y-axis (Yaw)
	glm::vec3 yawRotated;
	yawRotated.x = offset.x * cos(glm::radians(yaw)) - offset.z * sin(glm::radians(yaw));
	yawRotated.z = offset.z * cos(glm::radians(yaw)) + offset.x * sin(glm::radians(yaw));
	yawRotated.y = offset.y; // Y remains unchanged at this stage

	// Step 2: Rotate around X-axis (Pitch) AFTER applying yaw
	glm::vec3 finalRotated;
	finalRotated.x = yawRotated.x; // X remains unchanged in pitch rotation
	finalRotated.y = yawRotated.y * cos(glm::radians(pitch)) - yawRotated.z * sin(glm::radians(pitch));
	finalRotated.z = yawRotated.y * sin(glm::radians(pitch)) + yawRotated.z * cos(glm::radians(pitch));

	return finalRotated;
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//We normalize it because we dont care about how far does it go, we care about the direction so we use unit vectors to determine directions
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera()
{
}

