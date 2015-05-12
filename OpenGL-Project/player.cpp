//#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
using namespace std;

//#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.h"
#include "player.h"

#define START_POSITION vec3(0, 50, 150)
#define START_SPEED 75.0f
#define MOUSE_SENSITIVITY 0.0015f

Player::Player(Camera* camera)
{
	this->camera = camera;
	camera->setPosition(START_POSITION);
}

Camera* Player::getCamera()
{
	return camera;
}

void Player::setCamera(Camera* camera)
{
	this->camera = camera;
}

void Player::update(double delta)
{
	/*if (isButtonDown(GLFW_MOUSE_BUTTON_1))
	{
		setMouseLock(true);
	}

	if (isButtonDown(GLFW_MOUSE_BUTTON_2))
	{
		setMouseLock(false);
	}*/

	//if (isMouseLocked())
	//{
		vec2 mouseDelta = getMousePosition() * MOUSE_SENSITIVITY;

		float zRotation = 0.0f;
		//if (isKeyDown(GLFW_KEY_E)) zRotation += 1.0f * float(delta);
		//if (isKeyDown(GLFW_KEY_Q)) zRotation -= 1.0f * float(delta);

		camera->rotate(vec3(-mouseDelta.y, -mouseDelta.x, zRotation));
	//}

	vec3 movement = vec3();
	if (isKeyDown(GLFW_KEY_W)) movement += vec3(0, 0, 1);
	if (isKeyDown(GLFW_KEY_A)) movement -= vec3(1, 0, 0);
	if (isKeyDown(GLFW_KEY_S)) movement -= vec3(0, 0, 1);
	if (isKeyDown(GLFW_KEY_D)) movement += vec3(1, 0, 0);
	if (isKeyDown(GLFW_KEY_SPACE)) movement += vec3(0, 1, 0);
	if (isKeyDown(GLFW_KEY_LEFT_CONTROL)) movement -= vec3(0, 1, 0);

	if (length(movement) > 0.0f)
	{
		movement = normalize(movement);
		camera->move(movement * START_SPEED * float(delta));
	}
}