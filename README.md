# TinyShell – Phần của Sơn

## Files
| File | Vai trò |
|---|---|
| `son_commands.h` | Header: khai báo hàm public |
| `son_commands.c` | Implementation: tất cả lệnh của Sơn |
| `main.c`         | Vòng lặp REPL chính (tích hợp cả nhóm) |
| `Makefile`       | Build với MinGW |

## Biên dịch

```bash
# Dùng MinGW
mingw32-make

# Hoặc thủ công
gcc -Wall -o myShell.exe main.c son_commands.c -lkernel32
```

## Các lệnh Sơn implement

### Lệnh đặc biệt

| Lệnh | Mô tả |
|---|---|
| `help` | In danh sách lệnh của toàn shell |
| `exit` | Thoát myShell (gửi kill signal cho các background process – do Mạnh xử lý) |
| `date` | Hiển thị ngày hôm nay (`Thứ X, DD/MM/YYYY`) |
| `time` | Hiển thị giờ hiện tại (`HH:MM:SS.mmm`) |
| `dir [path]` | Liệt kê nội dung thư mục, kèm kích thước và ngày sửa đổi |

### Lệnh môi trường

| Lệnh | Mô tả |
|---|---|
| `path` | In từng entry của biến `PATH` (dạng có đánh số) |
| `addpath <dir>` | Thêm `<dir>` vào `PATH` cho phiên shell hiện tại |

> **Lưu ý:** `addpath` dùng `SetEnvironmentVariableA` – thay đổi chỉ có hiệu lực trong process myShell và các child process của nó; không ảnh hưởng đến registry Windows.

## Cách tích hợp với phần khác

`main.c` gọi `handle_son_command()` **trước tiên** trong vòng lặp.  
Hàm này trả về `1` nếu đã xử lý → `continue`.  
Nếu trả về `0` → chuyển xuống dispatcher của Huy / Mạnh / Cường.

```c
// Trong vòng lặp main của nhóm:
if (handle_son_command(trimmed, &should_exit))
    continue;
// ... các dispatcher khác ...
```

## Ví dụ chạy

```
myShell\C:\Users\Son>help
    ╔══════════════════════════════════════════╗
    ║         WELCOME TO MY SHELL              ║
    ╚══════════════════════════════════════════╝
  ...

myShell\C:\Users\Son>date
  Ngày hiện tại: Thứ năm, 14/05/2026

myShell\C:\Users\Son>time
  Giờ hiện tại: 09:32:17.045

myShell\C:\Users\Son>dir
  Nội dung thư mục: C:\Users\Son
  ...

myShell\C:\Users\Son>path
  [1] C:\Windows\system32
  [2] C:\Windows
  ...

myShell\C:\Users\Son>addpath C:\MyTools
  [addpath] Đã thêm 'C:\MyTools' vào PATH.

myShell\C:\Users\Son>exit
  Đang thoát myShell. Goodbye!
```
