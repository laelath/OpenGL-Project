#include "window.h"

using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Window::Window()
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

void Window::update()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

ivec2 Window::getSize() const
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return ivec2(width, height);
}

ivec2 Window::getFramebufferSize() const
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	return ivec2(width, height);
}

GLFWwindow* Window::getWindow() const
{
	return window;
}

void Window::setSize(unsigned int width, unsigned int height)
{
	glfwSetWindowSize(window, width, height);
}

void Window::setTitle(string title)
{
	glfwSetWindowTitle(window, title.c_str());
}

void Window::makeContextCurrent()
{
	glfwMakeContextCurrent(window);
}