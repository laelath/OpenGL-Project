#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
using namespace glm;

class Camera
{
public:
	Camera(/*vec3 position, quat orientation,*/ float zNear, float zFar);
	//Camera(/*vec3 position, vec3 rotation,*/ float zNear, float zFar);

	void setPosition(vec3 position);
	//void setOrientation(quat orientation);
	//void setRotation(vec3 rotation);
	//void setDirection(vec3 direction);
	//void setUp(vec3 up);
	void setRotation(quat rotation);
	void setRotation(vec3 rotation);
	void setPitch(float amount);
	void setYaw(float amount);
	void setRoll(float amount);

	void setZNear(float zNear);
	void setZFar(float zFar);

	void move(vec3 movement);
	void moveAxis(vec3 translation);

	//void rotate(quat rotation);
	void rotate(quat rotation);
	void rotate(vec3 rotation);
	void pitch(float amount);
	void yaw(float amount);
	void roll(float amount);

	vec3 getPosition() const;
	vec3 getDirection() const;
	vec3 getRight() const;
	vec3 getUp() const;
	//quat getOrientation() const;
	//vec3 getRotation() const;

	float getZNear() const;
	float getZFar() const;

	mat4 getViewMatrix() const;
	mat4 getProjectionMatrix() const;
	mat4 getViewProjectionMatrix() const;

protected:
	virtual void updateProjectionMatrix() = 0;

	float zNear;
	float zFar;

	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;

private:
	void updateViewMatrix();
	mat4 viewMatrix;

	vec3 position;
	//quat orientation;
	vec3 direction;
	vec3 right;
	vec3 up;
};

class Perspective_Camera : public Camera
{
public:

	Perspective_Camera(/*vec3 position, quat orientation,*/ float fov, float aspect_ratio, float zNear, float zFar);
	//Perspective_Camera(/*vec3 position, vec3 rotation,*/ float fov, float aspect_ratio, float zNear, float zFar);

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

	Orthogonal_Camera(/*vec3 position, quat orientation,*/ float left, float right, float bottom, float top, float zNear, float zFar);
	//Orthogonal_Camera(/*vec3 position, vec3 rotation,*/ float left, float right, float bottom, float top, float zNear, float zFar);

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