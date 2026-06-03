#include "process_manager.hpp"

#include <windows.h>
#include <cstdio>

// Danh sach background process dang chay
std::vector<BackgroundProcess> background_processes;

// Loai bo cac process da ket thuc
void cleanup_finished_processes()
{
    for (size_t i = 0; i < background_processes.size();)
    {
        HANDLE process_handle = (HANDLE)background_processes[i].process_handle;
        HANDLE thread_handle = (HANDLE)background_processes[i].thread_handle;

        DWORD exit_code = 0;
        if (GetExitCodeProcess(process_handle, &exit_code) && exit_code != STILL_ACTIVE)
        {
            // Process da ket thuc
            CloseHandle(process_handle);
            CloseHandle(thread_handle);
            background_processes.erase(background_processes.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

// In danh sach process dang chay
void print_running_processes()
{
    cleanup_finished_processes();

    if (background_processes.empty())
    {
        std::printf("  No background processes running.\n");
        return;
    }

    std::printf("  Background processes:\n");
    for (const auto &proc : background_processes)
    {
        std::printf("    PID: %u | Command: %s\n", proc.pid, proc.command);
    }
}

int handle_process_command(const char *cmd_line) {
    (void)cmd_line;
    std::printf("  [Manh] process management - chua implement\n");
    return 1;
}
