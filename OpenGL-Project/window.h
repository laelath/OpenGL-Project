#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window
{
public:
	Window();
	~Window();

	void update();

	glm::ivec2 getSize() const;
	glm::ivec2 getFramebufferSize() const;
	glm::ivec2 getPos() const;
	GLFWwindow* getWindow() const;
	
	void setSize(unsigned int width, unsigned int height);
	void setPos(int x, int y);
	void setTitle(std::string title);

	void makeContextCurrent();

private:
	GLFWwindow* window;
	unsigned int width, height;
};

