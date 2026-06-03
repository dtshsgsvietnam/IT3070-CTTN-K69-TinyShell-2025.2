#include "executor.hpp"

#include <windows.h>
#include <cstdio>
#include <cstring>
#include <string>

#include "process_manager.hpp"

// Kiem tra neu lenh ket thuc bang &
static bool is_background_command(const std::string &cmd, std::string &cmd_without_amp)
{
    // Tim & o cuoi lenh (sau khoang trang cuoi cung)
    size_t end = cmd.find_last_not_of(" \t\r\n");
    if (end != std::string::npos && cmd[end] == '&')
    {
        cmd_without_amp = cmd.substr(0, end);
        // Loai bo khoang trang cuoi cua phan lenh
        end = cmd_without_amp.find_last_not_of(" \t\r\n");
        if (end != std::string::npos)
        {
            cmd_without_amp = cmd_without_amp.substr(0, end + 1);
        }
        return true;
    }
    return false;
}

// Thuc hien lenh foreground (shell doi process ket thuc)
static void execute_foreground(const char *cmd_line)
{
    STARTUPINFOA startup_info = {};
    PROCESS_INFORMATION process_info = {};

    startup_info.cb = sizeof(STARTUPINFOA);

    // CreateProcessA khong thay doi cmd_line, nhung vi STARTUPINFO.lpReserved2 la NULL,
    // ta can dung CreateProcessA voi cmd_line dung la mutable hoac dung ShellExecuteA
    // De don gian, ta dung cmd.exe /c "command"
    char full_cmd[2048] = {};
    std::snprintf(full_cmd, sizeof(full_cmd), "cmd.exe /c \"%s\"", cmd_line);

    if (!CreateProcessA(
            nullptr,       // lpApplicationName
            full_cmd,      // lpCommandLine
            nullptr,       // lpProcessAttributes
            nullptr,       // lpThreadAttributes
            FALSE,         // bInheritHandles
            0,             // dwCreationFlags
            nullptr,       // lpEnvironment
            nullptr,       // lpCurrentDirectory
            &startup_info, // lpStartupInfo
            &process_info  // lpProcessInformation
            ))
    {
        DWORD error = GetLastError();
        std::printf("  Error: Cannot execute command (Error code: %lu)\n", error);
        return;
    }

    // Doi process ket thuc
    WaitForSingleObject(process_info.hProcess, INFINITE);

    // Lay exit code
    DWORD exit_code = 0;
    if (GetExitCodeProcess(process_info.hProcess, &exit_code))
    {
        if (exit_code != 0)
        {
            std::printf("  Process exited with code: %lu\n", exit_code);
        }
    }

    // Dong handle
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);
}

// Thuc hien lenh background (shell khong doi)
static void execute_background(const char *cmd_line)
{
    STARTUPINFOA startup_info = {};
    PROCESS_INFORMATION process_info = {};

    startup_info.cb = sizeof(STARTUPINFOA);

    char full_cmd[2048] = {};
    std::snprintf(full_cmd, sizeof(full_cmd), "cmd.exe /c \"%s\"", cmd_line);

    if (!CreateProcessA(
            nullptr,
            full_cmd,
            nullptr,
            nullptr,
            FALSE,
            CREATE_NEW_CONSOLE, // Tạo console riêng để không đè lên prompt chính
            nullptr,
            nullptr,
            &startup_info,
            &process_info))
    {
        DWORD error = GetLastError();
        std::printf("  Error: Cannot execute background command (Error code: %lu)\n", error);
        return;
    }

    // Luu process info
    BackgroundProcess bg_proc = {};
    bg_proc.pid = process_info.dwProcessId;
    std::strncpy(bg_proc.command, cmd_line, sizeof(bg_proc.command) - 1);
    bg_proc.process_handle = process_info.hProcess;
    bg_proc.thread_handle = process_info.hThread;
    bg_proc.is_stopped = false;

    background_processes.push_back(bg_proc);

    std::printf("  [%u] %s\n", bg_proc.pid, cmd_line);
}

// Xu ly lenh ngoai
int handle_external_command(const char *cmd_line)
{
    if (cmd_line == nullptr || cmd_line[0] == '\0')
    {
        return 1;
    }

    std::string cmd(cmd_line);
    std::string cmd_without_amp;

    // Kiem tra neu la lenh background
    bool is_background = is_background_command(cmd, cmd_without_amp);

    if (is_background)
    {
        execute_background(cmd_without_amp.c_str());
    }
    else
    {
        execute_foreground(cmd_line);
    }

    return 1;
}
