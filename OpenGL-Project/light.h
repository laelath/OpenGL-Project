#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
using namespace glm;

#include "shader.h"

#define SHADOW_RESOLUTION 2048
#define BIAS_MATRIX mat4(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0)

#define POINT_LIGHT_NAME "Point_Lights"
#define DIRECTIONAL_LIGHT_NAME "Directional_Lights"
#define SPOT_LIGHT_NAME "Spot_Lights"

void initLightRenderData();
const Shader* DepthProgram();
const GLuint ShadowSampler();

class Shadow_Light
{
public:
	~Shadow_Light();

	GLuint getFramebufferID() const;
	GLuint getTextureID() const;

	mat4 getViewMatrix() const;

	virtual void updateMatrices() = 0;
	virtual void bindRenderData() const = 0;
	virtual void bindLight(const Shader* program, unsigned int index, mat4 viewMatrix, GLuint texture_handle) const = 0;

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

	mat4 getProjectionMatrix(int index) const;

	void updateMatrices();
	void bindRenderData() const;
	void bindLight(const Shader* program, unsigned int index, mat4 viewMatrix, GLuint texture_handle) const;

protected:
	void initDepthBuffers();

private:
	mat4 projectionMatrices[6];
	mat4 viewProjectionMatrices[6];
};

class Directional_Light : public Shadow_Light
{
public:
	vec3 direction;
	vec4 color;

	Directional_Light();
	Directional_Light(vec3 direction, vec4 color);

	mat4 getProjectionMatrix() const;
	mat4 getViewProjectionMatrix() const;

	void updateMatrices();
	void bindRenderData() const;
	void bindLight(const Shader* program, unsigned int index, mat4 viewMatrix, GLuint texture_handle) const;

protected:
	void initDepthBuffers();

private:
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
};