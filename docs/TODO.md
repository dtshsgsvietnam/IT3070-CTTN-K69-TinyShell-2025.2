# Project 1: Tiny Shell (myShell)

## 1. Giới thiệu & Mục đích

### Giới thiệu
Dự án yêu cầu thiết kế và cài đặt một giao diện dòng lệnh (*shell*) đơn giản chạy trên hệ điều hành Windows, được đặt tên là **myShell**.

### Mục đích
* **Nghiên cứu API:** Tìm hiểu và trực tiếp áp dụng các hàm API quản lý tiến trình (Process Management) trong môi trường Windows.
* **Hiểu cơ chế hoạt động:** Nắm vững cách thức cài đặt cấu trúc vòng lặp điều khiển và phương thức hoạt động của một chương trình *shell* trong thực tế.

---

## 2. Yêu cầu tính năng (Nội dung thực hiện)

### A. Cơ chế nhận lệnh & Chế độ thực thi (Execution Modes)
Chương trình hoạt động theo chu kỳ liên tục: nhận chuỗi lệnh nhập vào, phân tích cú pháp (parse), và gọi API hệ điều hành để tạo **tiến trình con (child process)** thực thi. Hỗ trợ hai chế độ:
* **Foreground mode (Tiền cảnh):** Chế độ mặc định. Shell cha phải tạm dừng và đợi (wait) cho đến khi tiến trình con kết thúc hoàn toàn mới quay lại nhận lệnh tiếp theo.
* **Background mode (Hậu cảnh):** Thường kích hoạt khi có ký hiệu đặc biệt cuối lệnh (ví dụ: `&`). Shell cha và tiến trình con sẽ thực hiện song song; Shell lập tức hiển thị dấu nhắc cho người dùng nhập lệnh mới mà không cần đợi tiến trình con.

### B. Quản lý tiến trình chạy ngầm
Shell phải cung cấp các lệnh nội trú chuyên biệt để quản lý và kiểm soát các tiến trình đang chạy ở chế độ Background:
* **List:** In ra danh sách toàn bộ các tiến trình ngầm kèm thông tin chi tiết: `Process ID` (Mã định danh), `Cmd Name` (Tên lệnh/chương trình), và `Status` (Trạng thái hoạt động).
* **Kill:** Bắt buộc chấm dứt (terminate) một tiến trình ngầm đang chọn.
* **Stop:** Tạm dừng (pause) một tiến trình ngầm đang chạy.
* **Resume:** Tiếp tục cho phép tiến trình ngầm đang tạm dừng hoạt động trở lại.

### C. Các lệnh nội trú đặc biệt (Built-in Commands)
Shell cần tự xử lý nội bộ các lệnh sau thay vì chuyển tiếp cho hệ thống tạo tiến trình bên ngoài:
* **Lệnh cơ bản:** `exit` (thoát khỏi myShell), `help` (hiển thị hướng dẫn sử dụng), `date` (xem ngày hệ thống), `time` (xem giờ hệ thống), `dir` (liệt kê nội dung thư mục).
* **Quản lý môi trường:** `path` / `addpath` dùng để xem và thiết lập/bổ sung giá trị cho biến môi trường PATH, phục vụ việc tìm kiếm file thực thi.

### D. Xử lý tín hiệu ngắt từ bàn phím (Signal Handling)
* Shell phải bắt được tín hiệu từ tổ hợp phím **Ctrl + C** khi người dùng nhấn từ bàn phím.
* Ý nghĩa: Dùng để hủy bỏ (cancel/terminate) tiến trình *foreground* đang được thực thi.
* **Yêu cầu quan trọng:** Tín hiệu ngắt này chỉ tác động lên tiến trình con đang chạy tiền cảnh, tuyệt đối không được làm crash hoặc thoát chương trình `myShell` chính.

### E. Thực thi tệp lệnh Script (`*.bat`)
* Shell có khả năng chấp nhận đầu vào là một tệp tin văn bản có định dạng `*.bat`.
* Đọc và tự động thực thi tuần tự từng dòng lệnh chứa trong file đó như cách người dùng nhập thủ công từ bàn phím.