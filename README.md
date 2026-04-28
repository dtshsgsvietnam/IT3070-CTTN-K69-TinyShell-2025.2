# Project 1: Tiny Shell (myShell)

## 📝 Giới thiệu
**Tiny Shell (myShell)** là một chương trình mô phỏng giao diện dòng lệnh (command-line interface) đơn giản chạy trên nền tảng Windows. Dự án này được thiết kế nhằm mục đích nghiên cứu cách thức hoạt động của một shell, cách quản lý tiến trình (process management) và thực thi các API hệ thống của Windows.

## 🎯 Mục tiêu dự án
* Tìm hiểu và áp dụng các Windows API để quản lý tiến trình.
* Hiểu rõ cơ chế nhận lệnh, phân tích (parsing) và tạo tiến trình con.
* Quản lý vòng đời tiến trình (Foreground vs Background).

## ✨ Các tính năng chính
### 1. Chế độ thực thi
* **Foreground mode:** Shell sẽ đợi tiến trình con kết thúc mới tiếp tục nhận lệnh.
* **Background mode:** Shell và tiến trình con chạy song song.

### 2. Quản lý tiến trình
* `list`: Hiển thị danh sách các tiến trình đang chạy (ID, tên lệnh, trạng thái).
* `kill`, `stop`, `resume`: Điều khiển các tiến trình chạy ngầm.

### 3. Lệnh hệ thống tích hợp
* Các lệnh cơ bản: `exit`, `help`, `date`, `time`, `dir`.
* Quản lý môi trường: `path`, `addpath` (xem và thiết lập biến môi trường).

### 4. Xử lý tín hiệu & Tiện ích
* Hỗ trợ ngắt tín hiệu từ bàn phím (`Ctrl + C`) để hủy bỏ tiến trình đang chạy ở chế độ foreground.
* Hỗ trợ thực thi các tệp kịch bản `.bat`.

## 💻 Cài đặt và Sử dụng

### Yêu cầu hệ thống
* Hệ điều hành: Windows.
* Trình biên dịch: GCC (MinGW) hoặc MSVC (Visual Studio).

### Cách chạy chương trình
1.  **Biên dịch:** (Ví dụ sử dụng gcc)
    ```bash
    gcc main.c -o myShell.exe
    ```
2.  **Khởi chạy:**
    ```bash
    ./myShell.exe
    ```

## 👥 Thành viên thực hiện
| STT | Họ và Tên | MSSV | Nhiệm vụ |
|:---:|:---|:---:|:---|
| 1 |    | | Trưởng nhóm |
| 2 |    | | Thành viên |
| 3 |    | | Thành viên |
| 4 |    | | Thành viên |


## 📜 Tài liệu tham khảo
* Windows API Documentation (Process and Thread Functions).
* Giáo trình Hệ điều hành - Đại học Bách Khoa.
