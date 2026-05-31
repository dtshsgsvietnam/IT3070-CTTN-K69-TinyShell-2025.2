#pragma once

/*
 * TinyShell - Project 1
 * Phần của: Sơn  (C++ version)
 * Nội dung:
 *   - Các lệnh đặc biệt: exit, help, date, time, dir
 *   - path / addpath : xem và đặt lại biến môi trường PATH
 */

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

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
//  -1  : không phải lệnh của Sơn → chuyển xuống phần khác
int handle_son_command(const std::string &cmd_line);