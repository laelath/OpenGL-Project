#pragma once

#include "camera.h"

//void computeMatrices(GLFWwindow* window);
//mat4 getViewMatrix();
//mat4 getProjectionMatrix();
//vec3 getPlayerPos();
//vec3 getPlayerViewAngles();

class Player
{
public:
	Player(Camera* camera);

	Camera* getCamera();

	void setCamera(Camera* camera);
	void update(double delta);

private:
	Camera* camera;
};