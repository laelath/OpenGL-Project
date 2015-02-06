#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;


void computeMatrices(GLFWwindow* window);
mat4 getViewMatrix();
mat4 getProjectionMatrix();
vec3 getPlayerPos();