#pragma once

#include <string>

void addCommand(std::string name, void function(std::string));
void initConsole();
void submit(std::string submition);
void execute_queue();