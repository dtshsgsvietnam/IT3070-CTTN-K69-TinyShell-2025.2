# TinyShell

TinyShell là một shell đơn giản chạy trên Windows, viết bằng C++ và WinAPI.
Chương trình nhận lệnh từ người dùng theo vòng lặp REPL, xử lý các lệnh nội bộ
và có khung dispatcher để tích hợp các phần mở rộng của nhóm.

## Cấu trúc dự án

```text
TinyShell/
├── src/
│   ├── main.cpp              # Vòng lặp REPL chính và dispatcher của shell
│   ├── son_commands.cpp      # Cài đặt các lệnh built-in của Sơn
│   ├── huy_commands.cpp      # Placeholder phần foreground/background
│   ├── manh_commands.cpp     # Placeholder phần quản lý tiến trình
│   └── cuong_commands.cpp    # Placeholder phần chạy file .bat
├── include/
│   ├── son_commands.hpp      # Khai báo public cho module Sơn
│   ├── huy_commands.hpp      # Khai báo public cho module Huy
│   ├── manh_commands.hpp     # Khai báo public cho module Mạnh
│   └── cuong_commands.hpp    # Khai báo public cho module Cường
├── bin/
│   └── myShell.exe           # File thực thi sau khi build
├── Makefile                  # Cấu hình build bằng MinGW
├── README.md                 # Tài liệu dự án
└── .gitignore                # Các file/thư mục không đưa vào git
```

## Các file chính

| File | Vai trò |
|---|---|
| `src/main.cpp` | Chứa `main()`, in prompt, đọc lệnh và điều phối sang các module |
| `src/son_commands.cpp` | Cài đặt `help`, `exit`, `date`, `time`, `dir`, `path`, `addpath` |
| `src/huy_commands.cpp` | Stub cho phần chạy lệnh foreground/background |
| `src/manh_commands.cpp` | Stub cho phần quản lý tiến trình |
| `src/cuong_commands.cpp` | Stub cho phần thực thi file `.bat` |
| `include/son_commands.hpp` | Khai báo các hàm public của module Sơn |
| `include/huy_commands.hpp` | Khai báo hàm public của module Huy |
| `include/manh_commands.hpp` | Khai báo hàm public của module Mạnh |
| `include/cuong_commands.hpp` | Khai báo hàm public của module Cường |
| `Makefile` | Build toàn bộ source trong `src/` thành `bin/myShell.exe` |
| `.gitignore` | Bỏ qua output build, file tạm, file editor |

## Biên dịch

Yêu cầu: Windows + MinGW có `g++` và `mingw32-make`.

```bash
mingw32-make
```

File chạy sẽ được tạo tại:

```text
bin/myShell.exe
```

Có thể build thủ công bằng:

```bash
g++ -Wall -Wextra -std=c++17 -g -Iinclude -o bin/myShell.exe src/main.cpp src/son_commands.cpp src/huy_commands.cpp src/manh_commands.cpp src/cuong_commands.cpp -lkernel32
```

Xóa file build:

```bash
mingw32-make clean
```

## Chạy chương trình

```bash
bin\myShell.exe
```

Sau khi chạy, shell hiển thị prompt dạng:

```text
myShell\C:\path\to\current\directory>
```

Người dùng nhập lệnh tại prompt này.

## Luồng hoạt động

`src/main.cpp` chạy vòng lặp chính:

1. Lấy thư mục hiện tại bằng `GetCurrentDirectoryA`.
2. In prompt `myShell\<current_directory>`.
3. Đọc một dòng lệnh từ người dùng.
4. Bỏ qua dòng trống.
5. Gọi `handle_son_command()` để xử lý các lệnh built-in.
6. Nếu không phải lệnh của Sơn, dispatcher chuyển tiếp sang các phần khác:
   `handle_manh_command()`, `handle_cuong_command()`, hoặc `handle_huy_command()`.

Hiện tại các module Huy/Mạnh/Cường đã có file riêng trong `src/` và `include/`,
nhưng phần xử lý bên trong vẫn là stub để chờ tích hợp code thật.

## Các lệnh đã hỗ trợ

### Lệnh đặc biệt

| Lệnh | Mô tả |
|---|---|
| `help` | In danh sách lệnh |
| `exit` | Thoát khỏi myShell |
| `date` | Hiển thị ngày hiện tại |
| `time` | Hiển thị giờ hiện tại |
| `dir [path]` | Liệt kê nội dung thư mục |

### Lệnh môi trường

| Lệnh | Mô tả |
|---|---|
| `path` | In từng entry của biến môi trường `PATH` |
| `addpath <dir>` | Thêm thư mục vào `PATH` của phiên shell hiện tại |

Lưu ý: `addpath` dùng `SetEnvironmentVariableA`, nên thay đổi chỉ có hiệu lực
trong process `myShell` hiện tại và các child process của nó. Lệnh này không sửa
`PATH` trong registry Windows.

## Các phần đang chờ tích hợp

| Nhóm chức năng | Dispatcher | Trạng thái |
|---|---|---|
| Foreground/background execution | `handle_huy_command()` | Stub |
| Quản lý tiến trình: `list`, `kill`, `stop`, `resume` | `handle_manh_command()` | Stub |
| Thực thi file `.bat` | `handle_cuong_command()` | Stub |

## Ví dụ

```text
myShell\C:\Users\Son>help
myShell\C:\Users\Son>date
myShell\C:\Users\Son>time
myShell\C:\Users\Son>dir
myShell\C:\Users\Son>path
myShell\C:\Users\Son>addpath C:\MyTools
myShell\C:\Users\Son>exit
```
