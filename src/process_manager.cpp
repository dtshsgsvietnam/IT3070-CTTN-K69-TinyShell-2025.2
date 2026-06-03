#include "process_manager.hpp"
#include "shell.hpp"

#include <windows.h>
#include <tlhelp32.h>

#include <cstdio>
#include <cstdlib>
#include <string>

// Danh sach background process dang chay
std::vector<BackgroundProcess> background_processes;

static bool parse_pid(const std::string &text, uint32_t &pid)
{
    std::string value_text = trim_command(text);
    if (value_text.empty())
    {
        return false;
    }

    char *end = nullptr;
    unsigned long value = std::strtoul(value_text.c_str(), &end, 10);
    while (end != nullptr && (*end == ' ' || *end == '\t'))
    {
        end++;
    }

    if (end == value_text.c_str() || (end != nullptr && *end != '\0') || value == 0)
    {
        return false;
    }

    pid = static_cast<uint32_t>(value);
    return true;
}

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

static bool find_process_index(uint32_t pid, size_t &index)
{
    cleanup_finished_processes();

    for (size_t i = 0; i < background_processes.size(); i++)
    {
        if (background_processes[i].pid == pid)
        {
            index = i;
            return true;
        }
    }

    return false;
}

// In danh sach process dang chay
void print_running_processes()
{
    cleanup_finished_processes();

    if (background_processes.empty())
    {
        std::printf("  Khong co tien trinh nen nao dang chay.\n");
        return;
    }

    std::printf("  Danh sach tien trinh nen:\n");
    std::printf("  %-10s %-12s %s\n", "PID", "Trang thai", "Ten lenh");
    std::printf("  %-10s %-12s %s\n", "----------", "------------", "----------------");
    for (const auto &proc : background_processes)
    {
        std::printf("  %-10u %-12s %s\n",
                    proc.pid,
                    proc.is_stopped ? "Tam dung" : "Dang chay",
                    proc.command);
    }
}

static bool suspend_or_resume_process_threads(uint32_t pid, bool suspend)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    THREADENTRY32 thread_entry = {};
    thread_entry.dwSize = sizeof(THREADENTRY32);

    bool found_thread = false;
    bool success = true;

    if (Thread32First(snapshot, &thread_entry))
    {
        do
        {
            if (thread_entry.th32OwnerProcessID != pid)
            {
                continue;
            }

            found_thread = true;
            HANDLE thread_handle = OpenThread(THREAD_SUSPEND_RESUME, FALSE, thread_entry.th32ThreadID);
            if (thread_handle == nullptr)
            {
                success = false;
                continue;
            }

            DWORD result = suspend ? SuspendThread(thread_handle) : ResumeThread(thread_handle);
            if (result == static_cast<DWORD>(-1))
            {
                success = false;
            }

            CloseHandle(thread_handle);
        } while (Thread32Next(snapshot, &thread_entry));
    }
    else
    {
        success = false;
    }

    CloseHandle(snapshot);
    return found_thread && success;
}

static void kill_process(uint32_t pid)
{
    size_t index = 0;
    if (!find_process_index(pid, index))
    {
        std::printf("  [kill] Khong tim thay PID %u trong danh sach tien trinh nen.\n", pid);
        return;
    }

    HANDLE process_handle = static_cast<HANDLE>(background_processes[index].process_handle);
    HANDLE thread_handle = static_cast<HANDLE>(background_processes[index].thread_handle);

    if (!TerminateProcess(process_handle, 1))
    {
        std::printf("  [kill] Khong the ket thuc PID %u (ma loi: %lu).\n", pid, GetLastError());
        return;
    }

    WaitForSingleObject(process_handle, 1000);
    CloseHandle(process_handle);
    CloseHandle(thread_handle);
    background_processes.erase(background_processes.begin() + index);

    std::printf("  [kill] Da ket thuc PID %u.\n", pid);
}

static void stop_process(uint32_t pid)
{
    size_t index = 0;
    if (!find_process_index(pid, index))
    {
        std::printf("  [stop] Khong tim thay PID %u trong danh sach tien trinh nen.\n", pid);
        return;
    }

    if (background_processes[index].is_stopped)
    {
        std::printf("  [stop] PID %u da dang tam dung.\n", pid);
        return;
    }

    if (!suspend_or_resume_process_threads(pid, true))
    {
        std::printf("  [stop] Khong the tam dung PID %u.\n", pid);
        return;
    }

    background_processes[index].is_stopped = true;
    std::printf("  [stop] Da tam dung PID %u.\n", pid);
}

static void resume_process(uint32_t pid)
{
    size_t index = 0;
    if (!find_process_index(pid, index))
    {
        std::printf("  [resume] Khong tim thay PID %u trong danh sach tien trinh nen.\n", pid);
        return;
    }

    if (!background_processes[index].is_stopped)
    {
        std::printf("  [resume] PID %u van dang chay.\n", pid);
        return;
    }

    if (!suspend_or_resume_process_threads(pid, false))
    {
        std::printf("  [resume] Khong the tiep tuc PID %u.\n", pid);
        return;
    }

    background_processes[index].is_stopped = false;
    std::printf("  [resume] Da tiep tuc PID %u.\n", pid);
}

int handle_process_command(const char *cmd_line) {
    if (cmd_line == nullptr || cmd_line[0] == '\0')
    {
        return 1;
    }

    std::string command_line(cmd_line);
    std::string verb = to_lower(first_token(command_line));

    if (verb == "list")
    {
        print_running_processes();
        return 1;
    }

    uint32_t pid = 0;
    if (!parse_pid(command_args(command_line), pid))
    {
        std::printf("  Cu phap: %s <pid>\n", verb.c_str());
        return 1;
    }

    if (verb == "kill")
    {
        kill_process(pid);
    }
    else if (verb == "stop")
    {
        stop_process(pid);
    }
    else if (verb == "resume")
    {
        resume_process(pid);
    }
    else
    {
        std::printf("  Lenh quan ly tien trinh khong hop le: %s\n", verb.c_str());
    }

    return 1;
}
