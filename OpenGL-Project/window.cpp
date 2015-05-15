#include "window.h"
#include "input.h"
#include "console.h"

using namespace std;
using namespace glm;

#define WINDOW_SAMPLES 16

void window_focus_callback(GLFWwindow* window, int focused)
{
	if (focused == GL_TRUE)
	{
		bindWindow(window);
	}
}

void window_close_callback(GLFWwindow* window)
{
	submit("exit Window close requested.");
}

Window::Window(int width, int height, string title, GLFWmonitor* monitor, GLFWwindow* share)
{
	glfwWindowHint(GLFW_SAMPLES, WINDOW_SAMPLES);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title.c_str(), monitor, share);

	if (this->window == NULL) submit("print Error creating GLFW window.");

	glfwSetWindowFocusCallback(window, window_focus_callback);
	glfwSetWindowCloseCallback(window, window_close_callback);
}

Window::~Window()
{
	close();
}

void Window::update()
{

	glfwSwapBuffers(window);
}

ivec2 Window::size() const
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return ivec2(width, height);
}

ivec2 Window::framebufferSize() const
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	return ivec2(width, height);
}

ivec2 Window::position() const
{
	int xpos, ypos;
	glfwGetWindowPos(window, &xpos, &ypos);
	return ivec2(xpos, ypos);
}

GLFWwindow* Window::glfwWindow() const
{
	return window;
}

int Window::closeRequested() const
{
	return glfwWindowShouldClose(window);
}

void Window::setSize(unsigned int width, unsigned int height)
{
	glfwSetWindowSize(window, width, height);
}

void Window::setPos(int xpos, int ypos)
{
	glfwSetWindowPos(window, xpos, ypos);
}

void Window::setTitle(string title)
{
	glfwSetWindowTitle(window, title.c_str());
}

void Window::setShouldClose(bool should_close)
{
	glfwSetWindowShouldClose(window, should_close);
}

void Window::close()
{
	glfwDestroyWindow(window);
}

void Window::makeContextCurrent()
{
	glfwMakeContextCurrent(window);
}