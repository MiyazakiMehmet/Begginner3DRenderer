#pragma once

#include <stdio.h>

#include <GL/glew.h>

class ShadowMap
{
public:
	ShadowMap();

	virtual bool Init(unsigned int width, unsigned int height);

	virtual void Write();

	virtual void Read(GLenum textureUnit);

	unsigned int GetShadowWidth() { return shadowWidth; }
	unsigned int GetShadowHeight() { return shadowHeight; }
	unsigned int GetShadowMap() { return shadowMap; }




	~ShadowMap();

private:
	unsigned int FBO, shadowMap;
	unsigned int shadowWidth, shadowHeight;
};

