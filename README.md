# TinyShell

TinyShell là trình dòng lệnh đơn giản chạy trên Windows, viết bằng C++ và WinAPI.
Chương trình chạy theo vòng lặp REPL: in dấu nhắc, đọc lệnh, điều phối sang module phù hợp
rồi tiếp tục nhận lệnh mới.

## Cấu trúc dự án

```text
TinyShell/
├── src/
│   ├── main.cpp              # Điểm vào chương trình, REPL và dấu nhắc
│   ├── shell.cpp             # Phân tích và điều phối lệnh chung
│   ├── builtins.cpp          # help, exit, date, time, dir, path, addpath
│   ├── executor.cpp          # Chạy lệnh tiền cảnh/hậu cảnh
│   ├── process_manager.cpp   # Quản lý tiến trình nền: list/kill/stop/resume
│   └── script_runner.cpp     # Chạy từng dòng trong tệp .bat
├── include/
│   ├── shell.hpp
│   ├── builtins.hpp
│   ├── executor.hpp
│   ├── process_manager.hpp
│   └── script_runner.hpp
├── docs/
│   └── TODO.md               # Yêu cầu/tính năng cần thực hiện
├── Makefile                  # Biên dịch bằng MinGW
├── README.md
└── .gitignore
```

`bin/myShell.exe` là tệp kết quả biên dịch và được bỏ qua bởi `.gitignore`.

## Luồng xử lý

1. `main.cpp` thiết lập console UTF-8, in tiêu đề, in dấu nhắc và đọc một dòng lệnh.
2. `shell.cpp` cắt khoảng trắng thừa của lệnh và gọi `dispatch_command()`.
3. Thứ tự điều phối:
   - `builtins.cpp`: lệnh nội trú.
   - `process_manager.cpp`: `list`, `kill`, `stop`, `resume`.
   - `script_runner.cpp`: lệnh có từ đầu tiên kết thúc bằng `.bat`.
   - `executor.cpp`: các lệnh ngoài, gồm tiền cảnh và hậu cảnh với dấu `&`.

## Biên dịch và chạy

Yêu cầu: Windows + MinGW có `g++` và `mingw32-make`.

```bash
mingw32-make
bin\myShell.exe
```

Biên dịch thủ công tương đương:

```bash
g++ -Wall -Wextra -std=c++17 -g -Iinclude -o bin/myShell.exe src/main.cpp src/shell.cpp src/builtins.cpp src/executor.cpp src/process_manager.cpp src/script_runner.cpp -lkernel32
```

Xóa tệp biên dịch:

```bash
mingw32-make clean
```

## Tính năng hiện tại

Đã có:

| Nhóm | Lệnh/chức năng |
|---|---|
| Lệnh nội trú | `help`, `exit`, `date`, `time`, `dir [path]` |
| PATH | `path`, `addpath <dir>` |
| Thực thi lệnh ngoài | Tiền cảnh mặc định, hậu cảnh khi lệnh kết thúc bằng `&` |
| Quản lý tiến trình nền | `list`, `kill <pid>`, `stop <pid>`, `resume <pid>` |
| Ctrl+C | Hủy tiến trình tiền cảnh đang chạy, chương trình chính vẫn tiếp tục |
| Tệp lệnh `.bat` | Đọc tệp `.bat` và thực thi tuần tự từng dòng |

## Ví dụ

```text
myShell\C:\Project>help
myShell\C:\Project>dir
myShell\C:\Project>path
myShell\C:\Project>addpath C:\MyTools
myShell\C:\Project>ping 127.0.0.1
myShell\C:\Project>ping 127.0.0.1 -n 20 &
myShell\C:\Project>list
myShell\C:\Project>exit
```
