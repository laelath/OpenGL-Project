#include <iostream>
using namespace std;

#define GLM_SWIZZLE

#include <glm/gtc/matrix_transform.hpp>

#include "light.h"

Shadow_Light::~Shadow_Light()
{
	glDeleteFramebuffers(1, &framebufferID);
	glDeleteTextures(1, &depthTextureID);
}

GLuint Shadow_Light::getFramebufferID() const
{
	return framebufferID;
}

GLuint Shadow_Light::getTextureID() const
{
	return depthTextureID;
}

mat4 Shadow_Light::getViewMatrix() const
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

mat4 Point_Light::getProjectionMatrix(int index) const
{
	return projectionMatrices[index];
}

void Point_Light::updateMatrices()
{

}

void Point_Light::bindLight(const Shader* program, string name, mat4 viewMatrix, GLuint sampler, GLuint texture_handle)
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

mat4 Directional_Light::getProjectionMatrix() const
{
	return projectionMatrix;
}

mat4 Directional_Light::getViewProjectionMatrix() const
{
	return viewProjectionMatrix;
}

void Directional_Light::updateMatrices()
{
	projectionMatrix = ortho(-300.0f, 300.0f, -300.0f, 300.0f, -300.0f, 300.0f);
	viewMatrix = lookAt(direction, vec3(0, 0, 0), vec3(0, 1, 0));
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Directional_Light::bindLight(const Shader* program, string name, mat4 viewMatrix, GLuint sampler, GLuint texture_handle)
{
	program->uniform3f((viewMatrix * vec4(direction, 0)).xyz, name + ".direction");
	program->uniform4f(color, name + ".color");

	program->uniformMatrix4f(BIAS_MATRIX * viewProjectionMatrix, name + ".depthBiasMVP");

	glActiveTexture(GL_TEXTURE0 + texture_handle);
	glBindSampler(texture_handle, sampler);
	glBindTexture(GL_TEXTURE_2D, depthTextureID);
	program->uniform1i(texture_handle, name + ".depth_texture");
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