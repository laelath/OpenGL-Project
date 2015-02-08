#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
using namespace std;

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "input.h"
#include "player.h"

mat4 ViewMatrix;
mat4 ProjectionMatrix;
vec3 position = vec3(0, 50, 150);

mat4 getViewMatrix()
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

	if (isButtonDown(GLFW_MOUSE_BUTTON_1)/*glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS*/)
	{
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		setMouseLock(true);
		//glfwSetCursorPos(window, 0, 0);
	}

	if (isButtonDown(GLFW_MOUSE_BUTTON_2)/*glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS*/)
	{
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		setMouseLock(false);
	}

	if (isMouseLocked())
	{
		//double xpos, ypos;
		//glfwGetCursorPos(window, &xpos, &ypos);
		vec2 mousePos = getMousePos();

		//glfwSetCursorPos(window, 0, 0);

		horizontalAngle += mouseSpeed * float(mousePos.x);
		verticalAngle -= mouseSpeed * float(mousePos.y);
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

	if (isKeyDown(GLFW_KEY_LEFT_SHIFT)/*glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS*/)
	{
		speed += speed * deltaTime;
	}

	if (isKeyDown(GLFW_KEY_LEFT_CONTROL)/*glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS*/)
	{
		speed -= speed * deltaTime;
	}

	if (isKeyDown(GLFW_KEY_W)/*glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS*/)
	{
		position += direction * deltaTime * speed;
	}

	if (isKeyDown(GLFW_KEY_S)/*glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS*/)
	{
		position -= direction * deltaTime * speed;
	}

	if (isKeyDown(GLFW_KEY_D)/*glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS*/)
	{
		position += right * deltaTime * speed;
	}

	if (isKeyDown(GLFW_KEY_A)/*glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS*/)
	{
		position -= right * deltaTime * speed;
	}

	if (isKeyDown(GLFW_KEY_E)/*glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS*/)
	{
		position += up * deltaTime * speed;
	}

	if (isKeyDown(GLFW_KEY_Q)/*glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS*/)
	{
		position -= up * deltaTime * speed;
	}

	float FoV = initialFoV;

	ProjectionMatrix = perspective(radians(FoV), float(width) / float(height), 0.1f, 100000.0f);
	ViewMatrix = lookAt(position, position + direction, up);
	
	if (isKeyPressed(GLFW_KEY_ENTER)/*glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS*/)
	{
		cout << "Position: " << position.x << ", " << position.y << ", " << position.z << endl;
		cout << "Direction: " << direction.x << ", " << direction.y << ", " << direction.z << endl;
		cout << "View Angle: " << horizontalAngle << ", " << verticalAngle << endl << endl;
	}
}