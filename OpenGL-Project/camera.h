#pragma once

#include <glm/glm.hpp>
//#include <glm/gtx/quaternion.hpp>
using namespace glm;

class Camera
{
public:
	//Camera(vec3 position, quat direction, float zNear, float zFar);
	Camera(vec3 position, vec3 rotation, float zNear, float zFar);

	void setPosition(vec3 position);
	void setRotation(vec3 angles);
	void setDirection(quat direction);

	void setZNear(float zNear);
	void setZFar(float zFar);

	void translate(vec3 translation);
	void rotate(vec3 rotation);
	//void rotate(quat rotation);

	vec3 getPosition() const;
	vec3 getRotation() const;
	//quat getDirection() const;
	//vec3 getAngles() const;

	float getZNear() const;
	float getZFar() const;

	mat4 getViewMatrix() const;
	mat4 getProjectionMatrix() const;
	mat4 getViewProjectionMatrix() const;

protected:
	virtual void updateProjectionMatrix() = 0;

	vec3 position;
	vec3 rotation;
	//quat direction;

	float zNear;
	float zFar;

	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;

private:
	void updateViewMatrix();
	
	mat4 viewMatrix;
};

class Perspective_Camera : public Camera
{
public:
	//Perspective_Camera(vec3 position, quat direction, float fov, float aspect_ratio, float zNear, float zFar);
	Perspective_Camera(vec3 position, vec3 rotation, float fov, float aspect_ratio, float zNear, float zFar);

	void setFOV(float fov);
	void setAspectRatio(float aspect_ratio);

	float getFOV() const;
	float getAspectRatio() const;

protected:
	void updateProjectionMatrix();

private:
	float fov;
	float aspect_ratio;
};

class Orthogonal_Camera : public Camera
{
public:
	//Orthogonal_Camera(vec3 position, quat direction, float left, float right, float bottom, float top, float zNear, float zFar);
	Orthogonal_Camera(vec3 position, vec3 rotation, float left, float right, float bottom, float top, float zNear, float zFar);

	void setLeft(float left);
	void setRight(float right);
	void setBottom(float bottom);
	void setTop(float top);

	float getLeft() const;
	float getRight() const;
	float getBottom() const;
	float getTop() const;

protected:
	void updateProjectionMatrix();
	
private:
	float left;
	float right;
	float bottom;
	float top;
};