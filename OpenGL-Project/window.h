#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window
{
public:
	Window(int width, int height, std::string title, GLFWmonitor* monitor, GLFWwindow* share);
	~Window();

	void update();

	glm::ivec2 size() const;
	glm::ivec2 framebufferSize() const;
	glm::ivec2 position() const;
	GLFWwindow* glfwWindow() const;
	int closeRequested() const;
	
	void setSize(unsigned int width, unsigned int height);
	void setPos(int x, int y);
	void setTitle(std::string title);
	void setShouldClose(bool close);
	void close();

	void makeContextCurrent();

private:
	GLFWwindow* window;
	unsigned int width, height;
};

