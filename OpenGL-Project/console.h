#pragma once

#include <string>

void addCommand(std::string name, void function(std::string));
void initConsole();
void submit(std::string submission);
void submitImmediate(std::string submission);
void execute_queue();
bool isExitRequested();