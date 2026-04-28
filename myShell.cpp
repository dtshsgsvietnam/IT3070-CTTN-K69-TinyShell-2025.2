#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

struct ProcessInfo {
    DWORD pid;
    string command;
    HANDLE hProcess;
    HANDLE hThread;
};

vector<ProcessInfo> processList;

int handleBuiltInCommands(string input) {
    if (input == "exit") {
        return 0;
    }
    else if (input == "help") {
        cout << "myShell supports the following commands:\n";
        cout << "exit: Exit my shell\n";
        cout << "help: Print this help\n";
        cout << "list: List all background processes\n\n";
        return 1;
    }
    else if (input == "list") {
    	cout << "List Background Progress:\n";
    	
        for (auto it = processList.begin(); it != processList.end(); ) {
            DWORD exitCode;
            
            if (GetExitCodeProcess(it->hProcess, &exitCode) && exitCode == STILL_ACTIVE) {
                cout << "pid: " << it->pid << ", command: \"" << it->command << "\"\n";
                ++it;
            } 
            else {
                CloseHandle(it->hProcess);
                CloseHandle(it->hThread);
                it = processList.erase(it);
            }
        }
        if (processList.empty()) {
            cout << "There is no Background Progress!\n";
            return 1;
        }
        return 1;
    }
    return -1;
}

void executeCommand(string input) {
    bool isBackground = false;
    
    while (!input.empty() && input.back() == ' ') {
        input.pop_back();
    }
    
    // use '&' to use background mode
    if (!input.empty() && input.back() == '&') {
        isBackground = true;
        input.pop_back();
        while (!input.empty() && input.back() == ' ') {
            input.pop_back();
        }
    }	
        
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);    

    vector<char> cmdArgs(input.begin(), input.end());
    cmdArgs.push_back('\0');
    
    DWORD creationFlags = 0; 
    if (isBackground) {
        creationFlags = CREATE_NEW_CONSOLE; 
    }
    
    if (!CreateProcessA(
        NULL, cmdArgs.data(), NULL, NULL, true, creationFlags, NULL, NULL, &si, &pi
    )) {
        cout << "Bad command or file name...\n";
        return;
    }

    if (isBackground) {
        ProcessInfo newProc;
        newProc.pid = pi.dwProcessId;
        newProc.command = input;
        newProc.hProcess = pi.hProcess;
        newProc.hThread = pi.hThread;
        
        processList.push_back(newProc);
        cout << "[Background Process Started] PID: " << pi.dwProcessId << "\n";
    } 
    else {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        
        cin.clear();
    }
}


int main() {
    string input;
    cout<<"WELCOME TO MY SHELL\n\n";
    
    while (true) {
        cout << "myShell\>";
        getline(cin, input);
        
        if (input.empty()) continue;
        
        int status = handleBuiltInCommands(input);
        
        if (status == 0) {
            break;
        }
        else if (status == 1) {
            continue;
        }

        executeCommand(input);
    }

    return 0;
}