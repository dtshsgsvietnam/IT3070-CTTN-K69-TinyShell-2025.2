# TinyShell

TinyShell là shell đơn giản chạy trên Windows, viết bằng C++ và WinAPI. Chương
trình chạy theo vòng lặp REPL: in prompt, đọc lệnh, dispatch sang module phù hợp
rồi tiếp tục nhận lệnh mới.

## Cấu trúc dự án

```text
TinyShell/
├── src/
│   ├── main.cpp              # Điểm vào chương trình, REPL và prompt
│   ├── shell.cpp             # Parse/dispatch lệnh chung
│   ├── builtins.cpp          # help, exit, date, time, dir, path, addpath
│   ├── executor.cpp          # Chạy lệnh foreground/background
│   ├── process_manager.cpp   # Dữ liệu background process và stub list/kill/stop/resume
│   └── script_runner.cpp     # Stub chạy file .bat
├── include/
│   ├── shell.hpp
│   ├── builtins.hpp
│   ├── executor.hpp
│   ├── process_manager.hpp
│   └── script_runner.hpp
├── docs/
│   └── TODO.md               # Yêu cầu/tính năng cần thực hiện
├── Makefile                  # Build bằng MinGW
├── README.md
└── .gitignore
```

`bin/myShell.exe` là file build output và được ignore bởi `.gitignore`.

## Luồng xử lý

1. `main.cpp` set console UTF-8, in banner, in prompt và đọc một dòng lệnh.
2. `shell.cpp` trim lệnh và gọi `dispatch_command()`.
3. Thứ tự dispatch:
   - `builtins.cpp`: lệnh nội trú.
   - `process_manager.cpp`: `list`, `kill`, `stop`, `resume`.
   - `script_runner.cpp`: lệnh có token đầu kết thúc bằng `.bat`.
   - `executor.cpp`: các lệnh ngoài, gồm foreground và background với dấu `&`.

## Biên dịch và chạy

Yêu cầu: Windows + MinGW có `g++` và `mingw32-make`.

```bash
mingw32-make
bin\myShell.exe
```

Build thủ công tương đương:

```bash
g++ -Wall -Wextra -std=c++17 -g -Iinclude -o bin/myShell.exe src/main.cpp src/shell.cpp src/builtins.cpp src/executor.cpp src/process_manager.cpp src/script_runner.cpp -lkernel32
```

Xóa file build:

```bash
mingw32-make clean
```

## Tính năng hiện tại

Đã có:

| Nhóm | Lệnh/chức năng |
|---|---|
| Built-in | `help`, `exit`, `date`, `time`, `dir [path]` |
| PATH | `path`, `addpath <dir>` |
| Thực thi lệnh ngoài | Foreground mặc định, background khi lệnh kết thúc bằng `&` |

Đang chờ hoàn thiện:

| Nhóm | Trạng thái |
|---|---|
| Quản lý process nền | `list`, `kill`, `stop`, `resume` đang là stub |
| Script `.bat` | Đã có dispatcher, phần chạy từng dòng đang là stub |
| Ctrl+C | Chưa có handler riêng để hủy foreground process |

## Ví dụ

```text
myShell\C:\Project>help
myShell\C:\Project>dir
myShell\C:\Project>path
myShell\C:\Project>addpath C:\MyTools
myShell\C:\Project>ping 127.0.0.1
myShell\C:\Project>notepad &
myShell\C:\Project>exit
```
