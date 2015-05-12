#include "menu.h"

Menu::Menu()
{
}

Menu::~Menu()
{
	for (button* b : buttons)
	{
		delete(b);
	}

	for (background* b : backgrounds)
	{
		delete(b);
	}
}

void Menu::addButton(button* b)
{
	buttons.push_back(b);
}

void Menu::addButton(string text, vec4 color, GLuint image, vec2 position, vec2 size)
{
	button* b = new button;
	b->text = text;
	b->color = color;
	b->image = image;
	b->position = position;
	b->size = size;
	buttons.push_back(b);
}

void Menu::addBackground(background* b)
{
	backgrounds.push_back(b);
}

void Menu::addBackground(vec4 color, GLuint image, vec2 position, vec2 size)
{
	background* b = new background;
	b->color = color;
	b->image = image;
	b->position = position;
	b->size = size;
	backgrounds.push_back(b);
}