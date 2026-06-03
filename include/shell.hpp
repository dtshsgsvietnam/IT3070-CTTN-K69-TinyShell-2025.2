#pragma once

#include <string>

void print_banner(void);
std::string trim_command(const std::string &text);
bool dispatch_command(const std::string &cmd);
