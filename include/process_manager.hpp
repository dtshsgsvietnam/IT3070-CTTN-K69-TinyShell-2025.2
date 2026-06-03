#pragma once

#include <cstdint>
#include <vector>

// Cau truc luu thong tin background process
struct BackgroundProcess
{
    uint32_t pid;         // Process ID
    char command[512];    // Command line
    void *process_handle; // HANDLE to process (void* de tranh include windows.h)
    void *thread_handle;  // HANDLE to thread (void* de tranh include windows.h)
    bool is_stopped;      // true neu process dang bi tam dung
};

// Danh sach background process dang chay
extern std::vector<BackgroundProcess> background_processes;

int handle_process_command(const char *cmd_line);

// Ham tro giup
void cleanup_finished_processes();
void print_running_processes();
