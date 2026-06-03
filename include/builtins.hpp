#pragma once

/*
 * TinyShell - Project 1
 * Nội dung:
 *   - Các lệnh đặc biệt: exit, help, date, time, dir
 *   - path / addpath : xem và đặt lại biến môi trường PATH
 */

#include <string>

// Khai báo hàm public
void cmd_help();
void cmd_date();
void cmd_time_cmd();
void cmd_dir(const std::string &path = "");
void cmd_path();
void cmd_addpath(const std::string &new_dir);

// Dispatcher chính – trả về:
//   0  : lệnh "exit" → shell cần thoát
//   1  : đã xử lý, tiếp tục vòng lặp
//  -1  : không phải built-in command
int handle_builtin_command(const std::string &cmd_line);
