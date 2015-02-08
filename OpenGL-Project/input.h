#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

void initInput(GLFWwindow* window);
void updateInput();

void setMouseLock(bool mouseLock);

bool isMouseLocked();

bool isKeyDown(int key);
bool isKeyPressed(int key);
bool isKeyReleased(int key);

bool isButtonDown(int key);
bool isButtonPressed(int key);
bool isButtonReleased(int key);

vec2 getMousePos();