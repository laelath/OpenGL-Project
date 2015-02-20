#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "camera.h"

Camera::Camera(vec3 position, quat direction, float zNear, float zFar)
{
	this->position = position;
	this->direction = direction;
	this->zNear = zNear;
	this->zFar = zFar;
}

Camera::Camera(vec3 position, vec3 rotation, float zNear, float zFar)
{
	this->position = position;
	this->direction = quat(rotation);
	this->zNear = zNear;
	this->zFar = zFar;
}

void Camera::setPosition(vec3 position)
{
	this->position = position;
	updateMatrices();
}

void Camera::setDirection(quat direction)
{
	this->direction = direction;
	updateMatrices();
}

void Camera::setRotation(vec3 rotation)
{
	direction = quat(rotation);
	updateMatrices();
}

void Camera::translate(vec3 translation)
{
	position += translation;
	updateMatrices();
}

void Camera::rotate(quat rotation)
{
	direction = rotation * direction;
	updateMatrices();
}

void Camera::rotate(vec3 rotation)
{
	direction = quat(rotation) * direction;
}

vec3 Camera::getPosition() const
{
	return position;
}

quat Camera::getDirection() const
{
	return direction;
}

vec3 Camera::getAngles() const
{
	return eulerAngles(direction);
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



Perspective_Camera::Perspective_Camera(vec3 position, quat direction, float fov, float aspect_ratio, float zNear, float zFar)
	:Camera(position, direction, zNear, zFar)
{
	this->fov = fov;
	this->aspect_ratio = aspect_ratio;
	updateMatrices();
}

Perspective_Camera::Perspective_Camera(vec3 position, vec3 rotation, float fov, float aspect_ratio, float zNear, float zFar)
	:Camera(position, rotation, zNear, zFar)
{
	this->fov = fov;
	this->aspect_ratio = aspect_ratio;
	updateMatrices();
}

void Perspective_Camera::setFOV(float fov)
{
	this->fov = fov;
	updateMatrices();
}

void Perspective_Camera::setAspectRatio(float aspect_ratio)
{
	this->aspect_ratio = aspect_ratio;
	updateMatrices();
}

float Perspective_Camera::getFOV() const
{
	return fov;
}

float Perspective_Camera::getAspectRatio() const
{
	return aspect_ratio;
}

void Perspective_Camera::updateMatrices()
{
	viewMatrix = toMat4(direction) * glm::translate(mat4(), -position);
	projectionMatrix = perspective(fov, aspect_ratio, zNear, zFar);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}



Orthogonal_Camera::Orthogonal_Camera(vec3 position, quat direction, float left, float right, float bottom, float top, float zNear, float zFar)
	:Camera(position, direction, zNear, zFar)
{
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	updateMatrices();
}

Orthogonal_Camera::Orthogonal_Camera(vec3 position, vec3 rotation, float left, float right, float bottom, float top, float zNear, float zFar)
	:Camera(position, rotation, zNear, zFar)
{
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	updateMatrices();
}

void Orthogonal_Camera::setLeft(float left)
{
	this->left = left;
	updateMatrices();
}

void Orthogonal_Camera::setRight(float right)
{
	this->right = right;
	updateMatrices();
}

void Orthogonal_Camera::setBottom(float bottom)
{
	this->bottom = bottom;
	updateMatrices();
}

void Orthogonal_Camera::setTop(float top)
{
	this->top = top;
	updateMatrices();
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

void Orthogonal_Camera::updateMatrices()
{
	viewMatrix = toMat4(direction) * glm::translate(mat4(), position);
	projectionMatrix = ortho(left, right, bottom, top, zNear, zFar);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}