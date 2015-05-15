#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

#include "console.h"
#include "console_commands.h"

map<string, void(*)(string)> functions;
vector<pair<void(*)(string), string>> queue;
bool exit_requested;
mutex queue_mutex;

thread* console_thread;

void addCommand(string name, void function(string))
{
	if (functions.find(name) == functions.end())
		functions[name] = function;
	else
		cerr << "Tried to add console function that was already defined: " << name << endl;
}

void submit(string submission)
{
	string command = getStringCommand(submission);
	string content = getStringContent(submission);

	bool immediate = command == "imdt";
	if (immediate)
	{
		command = getStringCommand(content);
		content = getStringContent(content);
	}

	if (functions.find(command) != functions.end())
	{
		if (immediate)
			functions[command](content);
		else
			queue.push_back(make_pair(functions[command], content));
	}
	else
		cerr << "Unknown command, use \"help\" to view availible commands." << endl;
}

void execute_queue()
{
	queue_mutex.lock();
	for (pair<void(*)(string), string> function : queue)
	{
		function.first(function.second);
	}
	queue.clear();
	queue_mutex.unlock();
}

bool isExitRequested()
{
	return exit_requested;
}

void help(string content)
{
	int i = 0;
	for (pair<string, void(*)(string)> function : functions)
	{
		if (i % 3 == 2)
			cout << function.first << endl;
		else
			cout << function.first << "\t";
		i++;
	}
	cout << endl;
}

void exit(string content)
{
	cout << content;
	exit_requested = true;
}

void console()
{
	string submission;
	while (true)
	{
		cin >> submission;

		queue_mutex.lock();
		submit(submission);
		queue_mutex.unlock();
	}
}

void initConsole()
{
	addCommand("help", help);
	addCommand("exit", exit);
	addDefaultCommands();

	console_thread = new thread(console);
}
