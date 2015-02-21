#include <iostream>
using namespace std;

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
//#include <glm/gtc/quaternion.hpp>

#include "camera.h"

/*Camera::Camera(vec3 position, quat direction, float zNear, float zFar)
{
	this->position = position;
	this->direction = direction;
	this->zNear = zNear;
	this->zFar = zFar;
}*/

Camera::Camera(vec3 position, vec3 rotation, float zNear, float zFar)
{
	this->position = position;
	//this->direction = quat(rotation);
	this->rotation = rotation;
	this->zNear = zNear;
	this->zFar = zFar;
	updateViewMatrix();
}

void Camera::setPosition(vec3 position)
{
	this->position = position;
	updateViewMatrix();
}

/*void Camera::setDirection(quat direction)
{
	this->direction = direction;
	updateMatrices();
}*/

void Camera::setRotation(vec3 rotation)
{
	//direction = quat(rotation);
	this->rotation = rotation;
	updateViewMatrix();
}

void Camera::setZNear(float zNear)
{
	this->zNear = zNear;
	updateProjectionMatrix();
}

void Camera::setZFar(float zFar)
{
	this->zFar = zFar;
	updateProjectionMatrix();
}

void Camera::translate(vec3 translation)
{
	position += translation;
	updateViewMatrix();
}

/*void Camera::rotate(quat rotation)
{
	direction = rotation * direction;
	updateMatrices();
}*/

void Camera::rotate(vec3 rotation)
{
	//direction = quat(rotation) * direction;
	this->rotation += rotation;
	updateViewMatrix();
}

vec3 Camera::getPosition() const
{
	return position;
}

vec3 Camera::getRotation() const
{
	return rotation;
}

/*quat Camera::getDirection() const
{
	return direction;
}

vec3 Camera::getAngles() const
{
	return eulerAngles(direction);
}*/

float Camera::getZNear() const
{
	return zNear;
}

float Camera::getZFar() const
{
	return zFar;
}

mat4 Camera::getViewMatrix() const
{
	return viewMatrix;
}

mat4 Camera::getProjectionMatrix() const
{
	return projectionMatrix;
}

mat4 Camera::getViewProjectionMatrix() const
{
	return viewProjectionMatrix;
}

void Camera::updateViewMatrix()
{
	viewMatrix = orientate4(rotation) * glm::translate(mat4(), -position);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}



/*Perspective_Camera::Perspective_Camera(vec3 position, quat direction, float fov, float aspect_ratio, float zNear, float zFar)
	:Camera(position, direction, zNear, zFar)
{
	this->fov = fov;
	this->aspect_ratio = aspect_ratio;
	updateMatrices();
}*/

Perspective_Camera::Perspective_Camera(vec3 position, vec3 rotation, float fov, float aspect_ratio, float zNear, float zFar)
	:Camera(position, rotation, zNear, zFar)
{
	this->fov = fov;
	this->aspect_ratio = aspect_ratio;
	updateProjectionMatrix();
}

void Perspective_Camera::setFOV(float fov)
{
	this->fov = fov;
	updateProjectionMatrix();
}

void Perspective_Camera::setAspectRatio(float aspect_ratio)
{
	this->aspect_ratio = aspect_ratio;
	updateProjectionMatrix();
}

float Perspective_Camera::getFOV() const
{
	return fov;
}

float Perspective_Camera::getAspectRatio() const
{
	return aspect_ratio;
}

void Perspective_Camera::updateProjectionMatrix()
{
	projectionMatrix = perspective(fov, aspect_ratio, zNear, zFar);
	viewProjectionMatrix = projectionMatrix * getViewMatrix();
}

/*void Perspective_Camera::updateMatrices()
{
	viewMatrix = toMat4(direction) * glm::translate(mat4(), -position);
	projectionMatrix = perspective(fov, aspect_ratio, zNear, zFar);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}*/



/*Orthogonal_Camera::Orthogonal_Camera(vec3 position, quat direction, float left, float right, float bottom, float top, float zNear, float zFar)
	:Camera(position, direction, zNear, zFar)
{
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	updateMatrices();
}*/

Orthogonal_Camera::Orthogonal_Camera(vec3 position, vec3 rotation, float left, float right, float bottom, float top, float zNear, float zFar)
	:Camera(position, rotation, zNear, zFar)
{
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	updateProjectionMatrix();
}

void Orthogonal_Camera::setLeft(float left)
{
	this->left = left;
	updateProjectionMatrix();
}

void Orthogonal_Camera::setRight(float right)
{
	this->right = right;
	updateProjectionMatrix();
}

void Orthogonal_Camera::setBottom(float bottom)
{
	this->bottom = bottom;
	updateProjectionMatrix();
}

void Orthogonal_Camera::setTop(float top)
{
	this->top = top;
	updateProjectionMatrix();
}

float Orthogonal_Camera::getLeft() const
{
	return left;
}

float Orthogonal_Camera::getRight() const
{
	return right;
}

float Orthogonal_Camera::getBottom() const
{
	return bottom;
}

float Orthogonal_Camera::getTop() const
{
	return top;
}

void Orthogonal_Camera::updateProjectionMatrix()
{
	projectionMatrix = ortho(left, right, bottom, top, zNear, zFar);
	viewProjectionMatrix = projectionMatrix * getViewMatrix();
}

/*void Orthogonal_Camera::updateMatrices()
{
	viewMatrix = toMat4(direction) * glm::translate(mat4(), position);
	projectionMatrix = ortho(left, right, bottom, top, zNear, zFar);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}*/