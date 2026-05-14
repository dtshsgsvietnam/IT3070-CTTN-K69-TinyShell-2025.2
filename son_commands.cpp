/*
 * TinyShell - Project 1
 * Phần của: Sơn  (C++ version)
 *
 * Biên dịch cùng nhóm:
 *   g++ -Wall -o myShell.exe myshell.cpp son_commands.cpp -lkernel32
 */

#include "son_commands.hpp"
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

/* =========================================================
 * Helpers
 * ========================================================= */

// Chuyển string sang lowercase để so sánh không phân biệt hoa thường
static string to_lower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// Trim khoảng trắng hai đầu
static string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Tách lệnh thành [verb, args]
static pair<string, string> split_cmd(const string &line) {
    string t = trim(line);
    size_t sp = t.find_first_of(" \t");
    if (sp == string::npos)
        return { to_lower(t), "" };
    return { to_lower(t.substr(0, sp)), trim(t.substr(sp + 1)) };
}

/* =========================================================
 * 1. HELP
 * ========================================================= */
void cmd_help() {
    cout << "\n";
    cout << "    ╔══════════════════════════════════════════╗\n";
    cout << "    ║         WELCOME TO MY SHELL              ║\n";
    cout << "    ╚══════════════════════════════════════════╝\n\n";

    cout << "  myShell supports the following commands:\n\n";

    cout << "  ── Lệnh đặc biệt ──────────────────────────────────\n";
    cout << "  help          : Hiển thị danh sách lệnh này\n";
    cout << "  exit          : Thoát khỏi myShell\n";
    cout << "  date          : Hiển thị ngày hiện tại\n";
    cout << "  time          : Hiển thị giờ hiện tại\n";
    cout << "  dir [path]    : Liệt kê nội dung thư mục\n\n";

    cout << "  ── Biến môi trường ────────────────────────────────\n";
    cout << "  path          : Xem biến môi trường PATH\n";
    cout << "  addpath <dir> : Thêm <dir> vào PATH trong phiên này\n\n";

    cout << "  ── Quản lý tiến trình ─────────────────────────────\n";
    cout << "  list          : Liệt kê background processes\n";
    cout << "  kill   <pid>  : Kết thúc tiến trình\n";
    cout << "  stop   <pid>  : Tạm dừng tiến trình\n";
    cout << "  resume <pid>  : Tiếp tục tiến trình\n\n";

    cout << "  ── Thực thi ───────────────────────────────────────\n";
    cout << "  <cmd>         : Foreground (đợi kết thúc)\n";
    cout << "  <cmd> &       : Background (không đợi)\n";
    cout << "  <file>.bat    : Thực thi file batch\n\n";

    cout << "  ── Phím tắt ───────────────────────────────────────\n";
    cout << "  Ctrl+C        : Hủy foreground process\n\n";
}

/* =========================================================
 * 2. DATE
 * ========================================================= */
void cmd_date() {
    SYSTEMTIME st;
    GetLocalTime(&st);

    const char *days[] = {
        "Chủ nhật", "Thứ hai", "Thứ ba", "Thứ tư",
        "Thứ năm",  "Thứ sáu", "Thứ bảy"
    };

    cout << "  Ngày hiện tại: "
         << days[st.wDayOfWeek] << ", "
         << (st.wDay   < 10 ? "0" : "") << st.wDay   << "/"
         << (st.wMonth < 10 ? "0" : "") << st.wMonth << "/"
         << st.wYear << "\n";
}

/* =========================================================
 * 3. TIME
 * ========================================================= */
void cmd_time_cmd() {
    SYSTEMTIME st;
    GetLocalTime(&st);

    auto pad = [](int n) { return (n < 10 ? "0" : ""); };

    cout << "  Giờ hiện tại: "
         << pad(st.wHour)        << st.wHour   << ":"
         << pad(st.wMinute)      << st.wMinute << ":"
         << pad(st.wSecond)      << st.wSecond << "."
         << (st.wMilliseconds < 100 ? "0" : "")
         << (st.wMilliseconds < 10  ? "0" : "")
         << st.wMilliseconds << "\n";
}

/* =========================================================
 * 4. DIR
 * ========================================================= */
void cmd_dir(const string &path) {
    string target = path;

    if (target.empty()) {
        char buf[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buf);
        target = buf;
    }

    string search = target + "\\*";
    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(search.c_str(), &fd);

    if (hFind == INVALID_HANDLE_VALUE) {
        cout << "  [dir] Không tìm thấy: " << target << "\n";
        return;
    }

    cout << "\n  Nội dung thư mục: " << target << "\n";
    cout << "  " << string(70, '-') << "\n";
    cout << "  " << left;
    printf("  %-38s %-14s %s\n", "Tên", "Kích thước", "Ngày sửa đổi");
    cout << "  " << string(70, '-') << "\n";

    int file_count = 0, dir_count = 0;
    ULONGLONG total_bytes = 0;

    do {
        if (string(fd.cFileName) == "." || string(fd.cFileName) == "..") continue;

        SYSTEMTIME st;
        FILETIME local_ft = fd.ftLastWriteTime;
        FileTimeToLocalFileTime(&local_ft, &local_ft);
        FileTimeToSystemTime(&local_ft, &st);

        char date_buf[32];
        sprintf(date_buf, "%02d/%02d/%04d %02d:%02d",
                st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            printf("  %-38s %-14s %s\n", fd.cFileName, "<DIR>", date_buf);
            dir_count++;
        } else {
            ULONGLONG sz = ((ULONGLONG)fd.nFileSizeHigh << 32) | fd.nFileSizeLow;
            total_bytes += sz;

            char size_buf[20];
            if      (sz >= 1024*1024) sprintf(size_buf, "%.1f MB", sz / (1024.0*1024.0));
            else if (sz >= 1024)      sprintf(size_buf, "%.1f KB", sz / 1024.0);
            else                      sprintf(size_buf, "%llu B",  sz);

            printf("  %-38s %-14s %s\n", fd.cFileName, size_buf, date_buf);
            file_count++;
        }
    } while (FindNextFileA(hFind, &fd));

    FindClose(hFind);

    char total_buf[20];
    if      (total_bytes >= 1024*1024) sprintf(total_buf, "%.1f MB", total_bytes/(1024.0*1024.0));
    else if (total_bytes >= 1024)      sprintf(total_buf, "%.1f KB", total_bytes/1024.0);
    else                               sprintf(total_buf, "%llu B",  total_bytes);

    cout << "  " << string(70, '-') << "\n";
    cout << "  " << file_count << " file(s)  |  "
         << dir_count << " thư mục  |  Tổng: " << total_buf << "\n\n";
}

/* =========================================================
 * 5. PATH
 * ========================================================= */
void cmd_path() {
    const char *path_val = getenv("PATH");
    if (!path_val) {
        cout << "  [path] Không đọc được PATH.\n";
        return;
    }

    cout << "\n  Giá trị biến môi trường PATH:\n";
    cout << "  " << string(44, '=') << "\n";

    string path_str(path_val);
    stringstream ss(path_str);
    string token;
    int idx = 1;

    while (getline(ss, token, ';')) {
        // trim token
        string t = trim(token);
        if (!t.empty())
            cout << "  [" << (idx < 10 ? " " : "") << idx++ << "] " << t << "\n";
    }

    cout << "  " << string(44, '=') << "\n\n";
}

/* =========================================================
 * 6. ADDPATH
 * ========================================================= */
void cmd_addpath(const string &new_dir) {
    if (new_dir.empty()) {
        cout << "  [addpath] Cú pháp: addpath <thư mục>\n";
        return;
    }

    // Kiểm tra tồn tại
    DWORD attr = GetFileAttributesA(new_dir.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES) {
        cout << "  [addpath] Cảnh báo: '" << new_dir << "' không tồn tại.\n";
    } else if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        cout << "  [addpath] '" << new_dir << "' không phải thư mục.\n";
        return;
    }

    // Lấy PATH hiện tại
    char old_buf[8192] = {};
    GetEnvironmentVariableA("PATH", old_buf, sizeof(old_buf));
    string old_path(old_buf);

    // Kiểm tra trùng
    if (old_path.find(new_dir) != string::npos) {
        cout << "  [addpath] '" << new_dir << "' đã có trong PATH.\n";
        return;
    }

    string new_path = old_path.empty() ? new_dir : old_path + ";" + new_dir;
    SetEnvironmentVariableA("PATH", new_path.c_str());

    cout << "  [addpath] Đã thêm '" << new_dir << "' vào PATH.\n";
    cout << "  Lưu ý: chỉ có hiệu lực trong phiên shell này.\n";
}

/* =========================================================
 * 7. DISPATCHER
 *
 * Trả về:
 *   0  → exit
 *   1  → đã xử lý, continue
 *  -1  → không phải lệnh của Sơn
 * ========================================================= */
int handle_son_command(const string &cmd_line) {
    auto [verb, args] = split_cmd(cmd_line);

    if (verb == "help")    { cmd_help();             return 1; }
    if (verb == "exit")    { cout << "  Đang thoát myShell. Goodbye!\n"; return 0; }
    if (verb == "date")    { cmd_date();              return 1; }
    if (verb == "time")    { cmd_time_cmd();          return 1; }
    if (verb == "dir")     { cmd_dir(args);           return 1; }
    if (verb == "path")    { cmd_path();              return 1; }
    if (verb == "addpath") { cmd_addpath(args);       return 1; }

    return -1; // không phải lệnh của Sơn
}