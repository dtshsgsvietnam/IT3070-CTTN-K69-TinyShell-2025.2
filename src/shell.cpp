#include "shell.hpp"

#include <cctype>
#include <cstdio>
#include <string>

#include "builtins.hpp"
#include "executor.hpp"
#include "process_manager.hpp"
#include "script_runner.hpp"

std::string trim_command(const std::string &text)
{
    size_t start = 0;
    if (text.size() >= 3 &&
        static_cast<unsigned char>(text[0]) == 0xEF &&
        static_cast<unsigned char>(text[1]) == 0xBB &&
        static_cast<unsigned char>(text[2]) == 0xBF)
    {
        start = 3;
    }

    while (start < text.size() &&
           (text[start] == ' ' || text[start] == '\t' ||
            text[start] == '\r' || text[start] == '\n'))
    {
        start++;
    }

    size_t end = text.size();
    while (end > start &&
           (text[end - 1] == ' ' || text[end - 1] == '\t' ||
            text[end - 1] == '\r' || text[end - 1] == '\n'))
    {
        end--;
    }

    return text.substr(start, end - start);
}

std::string first_token(const std::string &line)
{
    std::string text = trim_command(line);
    size_t pos = text.find_first_of(" \t");
    if (pos == std::string::npos)
    {
        return text;
    }
    return text.substr(0, pos);
}

std::string to_lower(std::string text)
{
    for (char &ch : text)
    {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return text;
}

std::string command_args(const std::string &line)
{
    std::string text = trim_command(line);
    size_t pos = text.find_first_of(" \t");
    if (pos == std::string::npos)
    {
        return "";
    }
    return trim_command(text.substr(pos + 1));
}

static bool ends_with_bat(const std::string &token)
{
    if (token.size() < 4)
    {
        return false;
    }
    std::string ext = to_lower(token.substr(token.size() - 4));
    return ext == ".bat";
}

static std::string command_name_token(const std::string &cmd)
{
    std::string text = trim_command(cmd);
    if (!text.empty() && text[0] == '"')
    {
        size_t end_quote = text.find('"', 1);
        if (end_quote != std::string::npos)
        {
            return text.substr(1, end_quote - 1);
        }
    }

    return first_token(text);
}

static bool is_bat_command(const std::string &cmd)
{
    return ends_with_bat(command_name_token(cmd));
}

static bool is_process_command(const std::string &cmd)
{
    std::string verb = to_lower(first_token(cmd));
    return verb == "kill" || verb == "stop" ||
           verb == "resume" || verb == "list";
}

void print_banner(void)
{
    std::printf("\n");
    std::printf("  ============================================\n");
    std::printf("                  myShell v1.0                \n");
    std::printf("           Type 'help' to list commands       \n");
    std::printf("  ============================================\n\n");
}

bool dispatch_command(const std::string &cmd)
{
    int builtin_result = handle_builtin_command(cmd);
    if (builtin_result == 0)
    {
        return false;
    }
    if (builtin_result == 1)
    {
        return true;
    }

    if (is_process_command(cmd))
    {
        handle_process_command(cmd.c_str());
        return true;
    }

    if (is_bat_command(cmd))
    {
        return handle_script_command(cmd.c_str()) != 0;
    }

    handle_external_command(cmd.c_str());
    return true;
}
