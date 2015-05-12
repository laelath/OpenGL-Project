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
mutex queue_mutex;

thread* console_thread;

void addCommand(string name, void function(string))
{
	if (functions.find(name) == functions.end())
		functions[name] = function;
	else
		cerr << "Tried to add console function that was already defined: " << name << endl;
}

void submit(string submition)
{
	string command = submition.substr(0, submition.find(" "));
	string content = submition.substr(submition.find(" ") + 1, submition.length() - command.length() - 1);

	if (functions.find(command) != functions.end())
		queue.push_back(make_pair(functions[command], content)); //functions[command](content);
	else
		cerr << "Unknown command, use \"help\" to view commands." << endl;
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

void help(string content)
{
	int i = 0;
	for (pair<string, void(*)(string)> function : functions)
	{
		if (i % 2 == 0)
			cout << function.first << "\t";
		else
			cout << function.first << endl;
		i++;
	}
	cout << endl;
}

void console()
{
	string submition;
	while (true)
	{
		cin >> submition;

		queue_mutex.lock();
		submit(submition);
		queue_mutex.unlock();
	}
}

//thread console_thread(console);

void initConsole()
{
	addCommand("help", help);
	addDefaultCommands();

	console_thread = new thread(console);
}
