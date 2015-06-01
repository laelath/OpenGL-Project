#include <iostream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

#include "console_commands.h"
#include "engine.h"

vector<string> split(string str, char delimiter)
{
	vector<string> split_string;
	stringstream ss(str);
	string token;
	while (getline(ss, token, delimiter))
		split_string.push_back(token);
	return split_string;
}

void abort(string content)
{
	cerr << content;
	abort();
}

void create(string content)
{
	vector<string> commands = split(content, ' ');
	
	if (commands[0] == "scene")
	{

	}
	else if (commands[0] == "shader")
	{
		if (commands.size() == 4)
			addShader(commands[1], new Shader(commands[2], commands[3]));
		else if (commands.size() == 5)
			addShader(commands[1], new Shader(commands[2], commands[3], commands[4]));
		else
			cerr << "Call to load shader had incorrect number of arguments." << endl;
	}
	else if (commands[0] == "camera")
	{

	}
}

void set(string content)
{

}

void addDefaultCommands()
{
	addCommand("abort", abort);
	addCommand("create", create);
	addCommand("set", set);
}

string getStringCommand(string str)
{
	return str.substr(0, str.find(" "));
}

string getStringContent(string str)
{
	return (str.find(" ") == -1) ? "" : str.substr(str.find(" ") + 1);
	/*string content = "";
	int location = str.find(" ");
	if (location != -1)
		string content = str.substr(location + 1);
	return content;*/
}