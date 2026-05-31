/*
 * TinyShell - Project 1
 *
 * Main REPL and command dispatcher.
 */

#include <windows.h>

#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>

#include "cuong_commands.hpp"
#include "huy_commands.hpp"
#include "manh_commands.hpp"
#include "son_commands.hpp"

static std::string trim(const std::string &text) {
    size_t start = 0;
    while (start < text.size() &&
           (text[start] == ' ' || text[start] == '\t' ||
            text[start] == '\r' || text[start] == '\n')) {
        start++;
    }

    size_t end = text.size();
    while (end > start &&
           (text[end - 1] == ' ' || text[end - 1] == '\t' ||
            text[end - 1] == '\r' || text[end - 1] == '\n')) {
        end--;
    }

    return text.substr(start, end - start);
}

static std::string first_token(const std::string &line) {
    size_t pos = line.find_first_of(" \t");
    if (pos == std::string::npos) {
        return line;
    }
    return line.substr(0, pos);
}

static std::string to_lower(std::string text) {
    for (char &ch : text) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return text;
}

static bool ends_with_bat(const std::string &token) {
    if (token.size() < 4) {
        return false;
    }
    std::string ext = to_lower(token.substr(token.size() - 4));
    return ext == ".bat";
}

static bool is_bat_command(const std::string &cmd) {
    return ends_with_bat(first_token(cmd));
}

static bool is_manh_command(const std::string &cmd) {
    std::string verb = to_lower(first_token(cmd));
    return verb == "kill" || verb == "stop" ||
           verb == "resume" || verb == "list";
}

static void print_banner(void) {
    std::printf("\n");
    std::printf("  ============================================\n");
    std::printf("                  myShell v1.0                \n");
    std::printf("           Type 'help' to list commands       \n");
    std::printf("  ============================================\n\n");
}

int main(void) {
    char buffer[4096];

    print_banner();

    while (true) {
        char cwd[MAX_PATH] = {};
        if (!GetCurrentDirectoryA(MAX_PATH, cwd)) {
            std::strcpy(cwd, "?");
        }

        std::printf("myShell\\%s>", cwd);
        std::fflush(stdout);

        if (std::fgets(buffer, sizeof(buffer), stdin) == nullptr) {
            break;
        }

        std::string cmd = trim(buffer);
        if (cmd.empty()) {
            continue;
        }

        int son_result = handle_son_command(cmd);
        if (son_result == 0) {
            break;
        }
        if (son_result == 1) {
            continue;
        }

        if (is_manh_command(cmd)) {
            handle_manh_command(cmd.c_str());
            continue;
        }

        if (is_bat_command(cmd)) {
            handle_cuong_command(cmd.c_str());
            continue;
        }

        handle_huy_command(cmd.c_str());
    }

    return 0;
}
