#pragma once

#include <string>
#include <vector>
using namespace std;

#include <GL/glew.h>

#include <glm/glm.hpp>
using namespace glm;

struct button
{
	string text;
	GLuint image;
	vec4 color;
	vec2 position;
	vec2 size;
};

struct background
{
	GLuint image;
	vec4 color;
	vec2 position;
	vec2 size;
};

class Menu
{
public:
	Menu();
	~Menu();

	void draw() const;
	void update();

	void addButton(button* b);
	void addButton(string text, vec4 color, GLuint image, vec2 position, vec2 size);
	void addBackground(background* b);
	void addBackground(vec4 color, GLuint image, vec2 position, vec2 size);

private:
	vector<button*> buttons;
	vector<background*> backgrounds;
};