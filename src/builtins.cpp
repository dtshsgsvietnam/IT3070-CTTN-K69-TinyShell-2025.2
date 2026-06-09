/*
 * TinyShell - Project 1
 * Built-in commands.
 *
 * Build with the project Makefile:
 *   mingw32-make
 */

#include "builtins.hpp"
#include "shell.hpp"
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/* =========================================================
 * 1. HELP
 * ========================================================= */
void cmd_help() {
    cout << "\n";
    cout << "    ╔══════════════════════════════════════════╗\n";
    cout << "    ║           CHÀO MỪNG ĐẾN MYSHELL         ║\n";
    cout << "    ╚══════════════════════════════════════════╝\n\n";

    cout << "  myShell hỗ trợ các lệnh sau:\n\n";

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
    cout << "  list          : Liệt kê tiến trình nền\n";
    cout << "  kill   <pid>  : Kết thúc tiến trình\n";
    cout << "  stop   <pid>  : Tạm dừng tiến trình\n";
    cout << "  resume <pid>  : Tiếp tục tiến trình\n\n";

    cout << "  ── Thực thi ───────────────────────────────────────\n";
    cout << "  <lệnh>        : Chạy tiền cảnh (đợi kết thúc)\n";
    cout << "  <lệnh> &      : Chạy hậu cảnh (không đợi)\n";
    cout << "  <tệp>.bat     : Thực thi tệp lệnh .bat\n\n";

    cout << "  ── Phím tắt ───────────────────────────────────────\n";
    cout << "  Ctrl+C        : Hủy tiến trình tiền cảnh\n\n";
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
            printf("  %-38s %-14s %s\n", fd.cFileName, "<THƯ MỤC>", date_buf);
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
    cout << "  " << file_count << " tệp  |  "
         << dir_count << " thư mục  |  Tổng: " << total_buf << "\n\n";
}

/* =========================================================
 * 5. PATH
 * ========================================================= */
static string get_path_value() {
    DWORD needed = GetEnvironmentVariableA("PATH", nullptr, 0);
    if (needed == 0) {
        return "";
    }

    string value(needed, '\0');
    DWORD copied = GetEnvironmentVariableA("PATH", &value[0], needed);
    if (copied == 0 || copied >= needed) {
        return "";
    }

    value.resize(copied);
    return value;
}

static string normalize_path_entry(string path) {
    path = to_lower(trim_command(path));
    while (path.size() > 3 && (path.back() == '\\' || path.back() == '/')) {
        path.pop_back();
    }
    return path;
}

static bool path_has_entry(const string &path_value, const string &dir) {
    string target = normalize_path_entry(dir);
    if (target.empty()) {
        return false;
    }

    stringstream ss(path_value);
    string token;
    while (getline(ss, token, ';')) {
        if (normalize_path_entry(token) == target) {
            return true;
        }
    }

    return false;
}

void cmd_path() {
    string path_str = get_path_value();
    if (path_str.empty()) {
        cout << "  [path] Không đọc được PATH.\n";
        return;
    }

    cout << "\n  Giá trị biến môi trường PATH:\n";
    cout << "  " << string(44, '=') << "\n";

    stringstream ss(path_str);
    string token;
    int idx = 1;

    while (getline(ss, token, ';')) {
        // trim token
        string t = trim_command(token);
        if (!t.empty()) {
            cout << "  [" << (idx < 10 ? " " : "") << idx << "] " << t << "\n";
            idx++;
        }
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
    string old_path = get_path_value();

    // Kiểm tra trùng
    if (path_has_entry(old_path, new_dir)) {
        cout << "  [addpath] '" << new_dir << "' đã có trong PATH.\n";
        return;
    }

    string new_path = old_path.empty() ? new_dir : old_path + ";" + new_dir;
    if (!SetEnvironmentVariableA("PATH", new_path.c_str())) {
        cout << "  [addpath] Không thể cập nhật PATH.\n";
        return;
    }

    cout << "  [addpath] Đã thêm '" << new_dir << "' vào PATH.\n";
    cout << "  Lưu ý: chỉ có hiệu lực trong phiên shell này.\n";
}

/* =========================================================
 * 7. DISPATCHER
 *
 * Trả về:
 *   0  → exit
 *   1  → đã xử lý, continue
 *  -1  → không phải built-in command
 * ========================================================= */
int handle_builtin_command(const string &cmd_line) {
    string verb = to_lower(first_token(cmd_line));
    string args = command_args(cmd_line);

    if (verb == "help")    { cmd_help();             return 1; }
    if (verb == "exit")    { cout << "  Đang thoát myShell. Tạm biệt!\n"; return 0; }
    if (verb == "date")    { cmd_date();              return 1; }
    if (verb == "time")    { cmd_time_cmd();          return 1; }
    if (verb == "dir")     { cmd_dir(args);           return 1; }
    if (verb == "path")    { cmd_path();              return 1; }
    if (verb == "addpath") { cmd_addpath(args);       return 1; }

    return -1; // không phải lệnh của Sơn
}
