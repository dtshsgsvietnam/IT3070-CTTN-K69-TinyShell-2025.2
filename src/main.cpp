/*
 * TinyShell - Project 1
 *
 * Main REPL entry point.
 */

#include <windows.h>

#include <cstdio>
#include <cstring>
#include <string>

#include "shell.hpp"

int main(void)
{
    // Set UTF-8 mode for console
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    char buffer[4096];

    print_banner();

    while (true)
    {
        char cwd[MAX_PATH] = {};
        if (!GetCurrentDirectoryA(MAX_PATH, cwd))
        {
            std::strcpy(cwd, "?");
        }

        std::printf("myShell\\%s>", cwd);
        std::fflush(stdout);

        if (std::fgets(buffer, sizeof(buffer), stdin) == nullptr)
        {
            break;
        }

        std::string cmd = trim_command(buffer);
        if (cmd.empty())
        {
            continue;
        }

        if (!dispatch_command(cmd))
        {
            break;
        }
    }

    return 0;
}
