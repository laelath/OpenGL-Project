#pragma once

#include <glm/glm.hpp>
<<<<<<< HEAD
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
=======
//#include <glm/gtx/quaternion.hpp>
>>>>>>> origin/master
using namespace glm;

class Camera
{
public:
<<<<<<< HEAD
	Camera(vec3 position, quat orientation, float zNear, float zFar);
=======
	//Camera(vec3 position, quat direction, float zNear, float zFar);
>>>>>>> origin/master
	Camera(vec3 position, vec3 rotation, float zNear, float zFar);

	void setPosition(vec3 position);
	void setOrientation(vec3 rotation);
	void setOrientation(quat direction);

	void setZNear(float zNear);
	void setZFar(float zFar);

<<<<<<< HEAD
	void move(vec3 translation);
	void moveAxis(vec3 translation);
=======
	void translate(vec3 translation);
>>>>>>> origin/master
	void rotate(vec3 rotation);
	//void rotate(quat rotation);

	vec3 getPosition() const;
<<<<<<< HEAD
	quat getOrientation() const;
	vec3 getRotation() const;
=======
	vec3 getRotation() const;
	//quat getDirection() const;
	//vec3 getAngles() const;
>>>>>>> origin/master

	float getZNear() const;
	float getZFar() const;

	mat4 getViewMatrix() const;
	mat4 getProjectionMatrix() const;
	mat4 getViewProjectionMatrix() const;

protected:
	virtual void updateProjectionMatrix() = 0;

	vec3 position;
<<<<<<< HEAD
	quat orientation;
=======
	vec3 rotation;
>>>>>>> origin/master
	//quat direction;

	float zNear;
	float zFar;

	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;

private:
	void updateViewMatrix();
<<<<<<< HEAD
=======
	
>>>>>>> origin/master
	mat4 viewMatrix;
};

class Perspective_Camera : public Camera
{
public:
<<<<<<< HEAD
	Perspective_Camera(vec3 position, quat orientation, float fov, float aspect_ratio, float zNear, float zFar);
=======
	//Perspective_Camera(vec3 position, quat direction, float fov, float aspect_ratio, float zNear, float zFar);
>>>>>>> origin/master
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
<<<<<<< HEAD
	Orthogonal_Camera(vec3 position, quat orientation, float left, float right, float bottom, float top, float zNear, float zFar);
=======
	//Orthogonal_Camera(vec3 position, quat direction, float left, float right, float bottom, float top, float zNear, float zFar);
>>>>>>> origin/master
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