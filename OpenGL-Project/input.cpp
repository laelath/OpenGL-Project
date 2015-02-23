#include <iostream>
#include <vector>
using namespace std;

#include "input.h"

GLFWwindow* currentWindow;

vector<int> keysDown;
vector<int> keysPressed;
vector<int> keysReleased;

vector<int> buttonsDown;
vector<int> buttonsPressed;
vector<int> buttonsReleased;

vec2 mousePos;
bool locked;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		keysPressed.push_back(key);
		keysDown.push_back(key);
	}
	else if (action == GLFW_RELEASE)
	{
		keysReleased.push_back(key);
		keysDown.erase(find(keysDown.begin(), keysDown.end(), key));
	}
}

void button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		buttonsPressed.push_back(button);
		buttonsDown.push_back(button);
	}
	else if (action == GLFW_RELEASE)
	{
		buttonsReleased.push_back(button);
		buttonsDown.erase(find(buttonsDown.begin(), buttonsDown.end(), button));
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mousePos.x = float(xpos);
	mousePos.y = float(ypos);
}

void initInput(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	currentWindow = window;
}

void updateInput()
{
	keysPressed.clear();
	keysReleased.clear();
	buttonsPressed.clear();
	buttonsReleased.clear();

	if (locked)
	{
		glfwSetCursorPos(currentWindow, 0, 0);
		mousePos.x = 0.0;
		mousePos.y = 0.0;
	}

	glfwPollEvents();
}

void setMouseLock(bool mouseLock)
{
	locked = mouseLock;
	if (mouseLock)
	{
		glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(currentWindow, 0, 0);
		mousePos.x = 0.0;
		mousePos.y = 0.0;
	}
	else
	{
		glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

bool isMouseLocked()
{
	return locked;
}

bool isKeyDown(int key)
{
	if (find(keysDown.begin(), keysDown.end(), key) == keysDown.end()) return false;
	else return true;
}

bool isKeyPressed(int key)
{
	if (find(keysPressed.begin(), keysPressed.end(), key) == keysPressed.end()) return false;
	else return true;
}

bool isKeyReleased(int key)
{
	if (find(keysReleased.begin(), keysReleased.end(), key) == keysReleased.end()) return false;
	else return true;
}

bool isButtonDown(int button)
{
	if (find(buttonsDown.begin(), buttonsDown.end(), button) == buttonsDown.end()) return false;
	else return true;
}

bool isButtonPressed(int button)
{
	if (find(buttonsPressed.begin(), buttonsPressed.end(), button) == buttonsPressed.end()) return false;
	else return true;
}

bool isButtonReleased(int button)
{
	if (find(buttonsReleased.begin(), buttonsReleased.end(), button) == buttonsReleased.end()) return false;
	else return true;
}

vec2 getMousePosition()
{
	return mousePos;
}