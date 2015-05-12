#pragma once

#include <string>

void addCommand(std::string name, void function(std::string));
void initConsole();
void submit(std::string query);
void execute_queue();