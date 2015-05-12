#include <iostream>
using namespace std;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "camera.h"

Camera::Camera(/*vec3 position, quat orientation,*/ float zNear, float zFar)

{
	//this->position = position;
	//this->orientation = orientation;
	direction = vec3(0, 0, -1);
	right = vec3(1, 0, 0);
	up = vec3(0, 1, 0);

	this->zNear = zNear;
	this->zFar = zFar;
	updateViewMatrix();
}


/*Camera::Camera(vec3 position, vec3 rotation, float zNear, float zFar)
{
	this->position = position;

	/*quat xQuat = angleAxis(rotation.x, vec3(1, 0, 0));
	quat yQuat = angleAxis(rotation.y, vec3(0, 1, 0));
	quat zQuat = angleAxis(rotation.z, vec3(0, 0, 1));
	//this->orientation = xQuat * yQuat * zQuat;
	this->direction = ;

	this->zNear = zNear;
	this->zFar = zFar;
	updateViewMatrix();
}*/

void Camera::setPosition(vec3 position)
{
	this->position = position;
	updateViewMatrix();
}

/*void Camera::setOrientation(quat orientation)
{
	this->orientation = normalize(orientation);
	updateViewMatrix();
}*/

/*void Camera::setRotation(vec3 rotation)
{
	quat xQuat = angleAxis(rotation.x, vec3(1, 0, 0));
	quat yQuat = angleAxis(rotation.y, vec3(0, 1, 0));
	quat zQuat = angleAxis(rotation.z, vec3(0, 0, 1));
	this->orientation = normalize(yQuat * xQuat);
	updateViewMatrix();
}*/

void Camera::setRotation(quat rotation)
{
	direction = normalize(rotation * vec3(0, 0, -1));
	right = normalize(rotation * vec3(1, 0, 0));
	up = normalize(rotation * vec3(0, 1, 0));
}

void Camera::setRotation(vec3 rotation)
{
	setRoll(rotation.z);
	setPitch(rotation.x);
	setYaw(rotation.y);
}

void Camera::setPitch(float amount)
{
	quat rotation = angleAxis(amount, right);
	direction = normalize(rotation * vec3(0, 0, -1));
	if (dot(up, cross(right, direction)) < 0)
	{
		up *= -1;
	}
	updateViewMatrix();
}

void Camera::setYaw(float amount)
{
	quat rotation = angleAxis(amount, up);
	direction = normalize(rotation * vec3(0, 0, -1));
	right = normalize(rotation * vec3(1, 0, 0));
	updateViewMatrix();
}

void Camera::setRoll(float amount)
{
	quat rotation = angleAxis(amount, direction);
	right = normalize(rotation * vec3(1, 0, 0));
	up = normalize(rotation * vec3(0, 1, 0));
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

void Camera::move(vec3 movement)
{
	//position += (orientation * vec3(1, 0, 0)) * movement.x + (orientation * vec3(0, 1, 0)) * movement.y + (orientation * vec3(0, 0, 1)) * movement.z;
	position += right * movement.x + up * movement.y + direction * movement.z;
	updateViewMatrix();
}

void Camera::moveAxis(vec3 translation)
{
	position += translation;
	updateViewMatrix();
}

/*void Camera::rotate(quat rotation)
{
	orientation = normalize(rotation * orientation);
	updateViewMatrix();
}

void Camera::rotate(vec3 rotation)
{
	quat xQuat = angleAxis(rotation.x, vec3(1, 0, 0));
	quat yQuat = angleAxis(rotation.y, vec3(0, 1, 0));
	quat zQuat = angleAxis(rotation.z, vec3(0, 0, 1));
	orientation = normalize(yQuat * orientation * xQuat);
	updateViewMatrix();
}*/

void Camera::rotate(quat rotation)
{
	direction = normalize(rotation * direction);
	right = normalize(rotation * right);
	up = normalize(rotation * up);
}

void Camera::rotate(vec3 rotation)
{
	roll(rotation.z);
	pitch(rotation.x);
	yaw(rotation.y);
}

void Camera::pitch(float amount)
{
	quat rotation = angleAxis(amount, right);
	direction = normalize(rotation * direction);
	//up = normalize(rotation * up);
	if (dot(up, cross(right, direction)) < 0)
	{
		up *= -1;
	}
	updateViewMatrix();
}

void Camera::yaw(float amount)
{
	quat rotation = angleAxis(amount, up);
	direction = normalize(rotation * direction);
	right = normalize(rotation * right);
	updateViewMatrix();
}

void Camera::roll(float amount)
{
	quat rotation = angleAxis(amount, direction);
	right = normalize(rotation * right);
	up = normalize(rotation * up);
	updateViewMatrix();
}

vec3 Camera::getPosition() const
{
	return position;
}

/*quat Camera::getOrientation() const
{
	return orientation;
}

vec3 Camera::getRotation() const
{
	return eulerAngles(orientation);
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
	//viewMatrix = lookAt(position, position + (orientation * vec3(0, 0, -1)), (orientation * vec3(0, 1, 0)));
	viewMatrix = lookAt(position, position + direction, up/*cross(right, direction)*/);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}




Perspective_Camera::Perspective_Camera(/*vec3 position, quat orientation,*/ float fov, float aspect_ratio, float zNear, float zFar)
	:Camera(/*position, orientation,*/ zNear, zFar)
{
	this->fov = fov;
	this->aspect_ratio = aspect_ratio;
	updateProjectionMatrix();
}

/*Perspective_Camera::Perspective_Camera(vec3 position, vec3 rotation, float fov, float aspect_ratio, float zNear, float zFar)
	:Camera(position, rotation, zNear, zFar)
{
	this->fov = fov;
	this->aspect_ratio = aspect_ratio;
	updateProjectionMatrix();
}*/

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
	projectionMatrix = perspective(radians(fov), aspect_ratio, zNear, zFar);
	viewProjectionMatrix = projectionMatrix * getViewMatrix();
}



Orthogonal_Camera::Orthogonal_Camera(/*vec3 position, quat orientation,*/ float left, float right, float bottom, float top, float zNear, float zFar)
	:Camera(/*position, orientation,*/ zNear, zFar)
{
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;

	updateProjectionMatrix();
}

/*Orthogonal_Camera::Orthogonal_Camera(vec3 position, vec3 rotation, float left, float right, float bottom, float top, float zNear, float zFar)
	:Camera(position, rotation, zNear, zFar)
{
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	updateProjectionMatrix();
}*/

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