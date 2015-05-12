#include <stdlib.h>

#include <iostream>
#include <string>
#include <map>
using namespace std;

#include <GLFW/glfw3.h>
#include "console_commands.h"
#include "console.h"

void exit(string content)
{
	cout << content << endl;

	//glfwSetWindowShouldClose(window, true);
}

void create(string content)
{
	string object = content.substr(0, content.find(" "));
	string path = content.substr(content.find(" ") + 1, content.length() - object.length() - 1);


}

void addDefaultCommands()
{
	//addCommand("exit", exit);
	addCommand("create", create);
}
