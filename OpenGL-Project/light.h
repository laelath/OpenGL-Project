#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
using namespace glm;

#define SHADOW_RESOLUTION 4096
#define BIAS_MATRIX mat4(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0)

class Shadow_Light
{
public:
	GLuint getFramebufferID();
	GLuint getTextureID();

	mat4 getViewMatrix();

	virtual void updateMatrices() = 0;

protected:
	GLuint framebufferID, depthTextureID;

	mat4 viewMatrix;

	virtual void initDepthBuffers() = 0;
};

class Point_Light : public Shadow_Light
{
public:
	vec3 position;
	vec4 color;

	Point_Light();
	Point_Light(vec3 position, vec4 color);

	mat4 getProjectionMatrix(int index);

	void updateMatrices();

protected:
	void initDepthBuffers();

private:
	mat4 projectionMatrices[6];
};

class Directional_Light : public Shadow_Light
{
public:
	vec3 direction;
	vec4 color;

	Directional_Light();
	Directional_Light(vec3 direction, vec4 color);

	mat4 getProjectionMatrix();
	mat4 getViewProjectionMatrix();

	void updateMatrices();

protected:
	void initDepthBuffers();

private:
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
};