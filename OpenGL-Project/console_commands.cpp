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
	vector<string> commands = split(content);
	//string object = getStringCommand(content);
	//vector<string> params = split(getStringContent(content));
	
	if (commands[0] == "scene")
	{

	}
	else if (commands[0] == "shader")
	{
		if (commands.size() == 3)
			addShader(new Shader(commands[1], commands[2]));
		else if (commands.size() == 4)
			addShader(new Shader(commands[1], commands[2], commands[3]));
		else
			cerr << "Call to load shader had incorrect number of arguments." << endl;
	}
	else if (commands[0] == "camera")
	{

	}
}

void addDefaultCommands()
{
	addCommand("abort", abort);
	addCommand("create", create);
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