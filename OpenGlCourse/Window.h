#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowheight);

	int Initialize();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeigth() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();
private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	//Covers ascii 
	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void CreateCallbacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

