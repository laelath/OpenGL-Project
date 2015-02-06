#include <iostream>
using namespace std;

#include <glm/gtc/matrix_transform.hpp>

#include "light.h"

GLuint Shadow_Light::getFramebufferID()
{
	return framebufferID;
}

GLuint Shadow_Light::getTextureID()
{
	return depthTextureID;
}

mat4 Shadow_Light::getViewMatrix()
{
	return viewMatrix;
}


Point_Light::Point_Light()
{
	initDepthBuffers();
	position = vec3();
	color = vec4();
	updateMatrices();
}

Point_Light::Point_Light(vec3 position, vec4 color)
{
	initDepthBuffers();
	this->position = position;
	this->color = color;
	updateMatrices();
}

mat4 Point_Light::getProjectionMatrix(int index)
{
	return projectionMatrices[index];
}

void Point_Light::updateMatrices()
{

}

void Point_Light::initDepthBuffers()
{

}


Directional_Light::Directional_Light()
{
	initDepthBuffers();
	direction = vec3(0, 1, 0);
	color = vec4();
	updateMatrices();
}

Directional_Light::Directional_Light(vec3 direction, vec4 color)
{
	initDepthBuffers();
	this->direction = normalize(direction);
	this->color = color;
	updateMatrices();
}

mat4 Directional_Light::getProjectionMatrix()
{
	return projectionMatrix;
}

mat4 Directional_Light::getViewProjectionMatrix()
{
	return viewProjectionMatrix;
}

void Directional_Light::updateMatrices()
{
	projectionMatrix = ortho(-300.0f, 300.0f, -300.0f, 300.0f, -300.0f, 300.0f);
	viewMatrix = lookAt(direction, vec3(0, 0, 0), vec3(0, 1, 0));
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Directional_Light::initDepthBuffers()
{
	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

	glGenTextures(1, &depthTextureID);
	glBindTexture(GL_TEXTURE_2D, depthTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_RESOLUTION, SHADOW_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureID, 0);
	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cerr << "Error creating directional light framebuffer." << endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}