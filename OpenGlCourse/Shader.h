#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "gl/glew.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "CommonValues.h"
#include "Spotlight.h"

class Shader
{
public:
	Shader();

	//One gets shader from string other one gets it from file
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetViewLocation();
	GLuint GetModelLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetEyePositionLocation();

	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount);
	void SetSpotLights(SpotLight* sLight, unsigned int lightCount);

	void UseShader();
	void ClearShader();

	~Shader();
private:
	int pointLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView,
		//uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection 
		uniformSpecularIntensity, uniformShininess, uniformEyePosition;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	}uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	}uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;

	}uniformSpotLight[MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

};

