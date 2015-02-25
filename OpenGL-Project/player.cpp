//#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
using namespace std;

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.h"
#include "player.h"

//mat4 ViewMatrix;
//mat4 ProjectionMatrix;
//vec3 position = vec3(0, 50, 150);

#define START_POSITION vec3(0, 50, 150)
#define START_SPEED 75.0f
#define MOUSE_SENSITIVITY 0.0015f

//const float Vertical_View_Limit = radians(89.99f);
//const float Horizontal_View_Limit = radians(180.0f);

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
	if (isButtonDown(GLFW_MOUSE_BUTTON_1))
	{
		setMouseLock(true);
	}

	if (isButtonDown(GLFW_MOUSE_BUTTON_2))
	{
		setMouseLock(false);
	}

	if (isMouseLocked())
	{
		vec2 mouseDelta = getMousePosition() * MOUSE_SENSITIVITY;

		float zRotation = 0.0f;
		//if (isKeyDown(GLFW_KEY_E)) zRotation += 1.0f * float(delta);
		//if (isKeyDown(GLFW_KEY_Q)) zRotation -= 1.0f * float(delta);

		camera->rotate(vec3(-mouseDelta.y, -mouseDelta.x, zRotation));
	}

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

//float horizontalAngle = 0.0f;
//float verticalAngle = 0.0f;

/*mat4 getViewMatrix()
{
	return ViewMatrix;
}

mat4 getProjectionMatrix()
{
	return ProjectionMatrix;
}

vec3 getPlayerPos()
{
	return position;
}

vec3 getPlayerViewAngles()
{
	return vec3(horizontalAngle, verticalAngle, 0);
}*/

//float initialFoV = 90.0f;

//float speed = 50.0f;
//float mouseSpeed = 0.001f;
//float speedMultiplyer = 1.0f;



/*void computeMatrices(GLFWwindow* window)
{
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	lastTime = currentTime;

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	if (isButtonDown(GLFW_MOUSE_BUTTON_1))
	{
		setMouseLock(true);
	}

	if (isButtonDown(GLFW_MOUSE_BUTTON_2))
	{
		setMouseLock(false);
	}

	if (isMouseLocked())
	{
		vec2 mousePos = getMousePosition();



		//horizontalAngle += mouseSpeed * float(mousePos.x);
		//verticalAngle -= mouseSpeed * float(mousePos.y);
	}
	
	float verticalLimit = radians(89.99f);

	if (verticalAngle > verticalLimit)
	{
		verticalAngle = verticalLimit;
	}
	else if (verticalAngle < -verticalLimit)
	{
		verticalAngle = -verticalLimit;
	}

	float horizontalLimit = radians(180.0f);
	
	if (horizontalAngle > horizontalLimit)
	{
		horizontalAngle -= radians(360.0f);
	}
	else if (horizontalAngle < -horizontalLimit)
	{
		horizontalAngle += radians(360.0f);
	}

	vec3 direction(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), -cos(verticalAngle) * cos(horizontalAngle));
	//vec3 right = vec3(sin(horizontalAngle + M_PI_2), 0, cos(horizontalAngle - M_PI_2));
	//vec3 up = cross(right, direction);
	vec3 up = vec3(0, 1, 0);
	vec3 right = normalize(cross(direction, up));

	if (isKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		speed += speed * deltaTime;
	}

	if (isKeyDown(GLFW_KEY_LEFT_CONTROL))
	{
		speed -= speed * deltaTime;
	}

	if (isKeyDown(GLFW_KEY_W))
	{
		position += direction * deltaTime * speed;
	}

	if (isKeyDown(GLFW_KEY_S))
	{
		position -= direction * deltaTime * speed;
	}

	if (isKeyDown(GLFW_KEY_D))
	{
		position += right * deltaTime * speed;
	}

	if (isKeyDown(GLFW_KEY_A))
	{
		position -= right * deltaTime * speed;
	}

	if (isKeyDown(GLFW_KEY_E))
	{
		position += up * deltaTime * speed;
	}

	if (isKeyDown(GLFW_KEY_Q))
	{
		position -= up * deltaTime * speed;
	}

	float FoV = initialFoV;

	ProjectionMatrix = perspective(radians(FoV), float(width) / float(height), 0.1f, 100000.0f);
	ViewMatrix = lookAt(position, position + direction, up);
	
	if (isKeyPressed(GLFW_KEY_ENTER))
	{
		cout << "Position: " << position.x << ", " << position.y << ", " << position.z << endl;
		cout << "Direction: " << direction.x << ", " << direction.y << ", " << direction.z << endl;
		cout << "View Angle: " << horizontalAngle << ", " << verticalAngle << endl << endl;
	}
}*/