#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
using namespace std;

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.h"

mat4 ViewMatrix;
mat4 ProjectionMatrix;

mat4 getViewMatrix()
{
	return ViewMatrix;
}
mat4 getProjectionMatrix()
{
	return ProjectionMatrix;
}

vec3 position = vec3(0, 50, 150);
float horizontalAngle = 0.0f;
float verticalAngle = 0.0f;
float initialFoV = 90.0f;

float speed = 50.0f;
float mouseSpeed = 0.001f;
float speedMultiplyer = 1.0f;

void computeMatrices(GLFWwindow* window)
{
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	lastTime = currentTime;

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, 0, 0);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		glfwSetCursorPos(window, 0, 0);

		horizontalAngle += mouseSpeed * float(xpos);
		verticalAngle -= mouseSpeed * float(ypos);
	}
	
	float verticalLimit = radians(90.0f);

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
	vec3 left = vec3(sin(horizontalAngle + M_PI_2), 0, cos(horizontalAngle - M_PI_2));
	vec3 up = cross(left, direction);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed += speed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		speed -= speed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += direction * deltaTime * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= direction * deltaTime * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += left * deltaTime * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= left * deltaTime * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		position += up * deltaTime * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		position -= up * deltaTime * speed;
	}

	float FoV = initialFoV;

	ProjectionMatrix = perspective(radians(FoV), float(width) / float(height), 0.1f, 100000.0f);
	//ProjectionMatrix = ortho(-float(width) * 0.25f, float(width) * 0.25f, -float(height) * 0.25f, float(height) * 0.25f, 1000.0f, -1000.0f);
	ViewMatrix = lookAt(position, position + direction, up);
	
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		cout << position.x << ", " << position.y << ", " << position.z << endl;
		cout << direction.x << ", " << direction.y << ", " << direction.z << endl;
		cout << horizontalAngle << ", " << verticalAngle << endl;
	}
}