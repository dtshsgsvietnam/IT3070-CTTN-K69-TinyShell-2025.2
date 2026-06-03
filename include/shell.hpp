#pragma once

#include <string>

void print_banner(void);

// Helper parse chung cho cac module xu ly lenh
std::string trim_command(const std::string &text);
std::string first_token(const std::string &line);
std::string to_lower(std::string text);
std::string command_args(const std::string &line);

bool dispatch_command(const std::string &cmd);
