# TODO - Phan chia nhiem vu TinyShell

Tai lieu nay tong hop phan cong tu `Project-1.docx` va gan voi cau truc hien tai cua du an.

## Trang thai chung

- [x] Tao khung shell chinh trong `src/main.cpp`
- [x] Tach source vao `src/`, header vao `include/`, tai lieu vao `docs/`
- [x] Them `Makefile` build ra `bin/myShell.exe`
- [x] Them `.gitignore`
- [x] Tao module placeholder cho Huy/Manh/Cuong trong `src/` va `include/`
- [x] Tich hop cac module Huy/Manh/Cuong vao dispatcher trong `src/main.cpp`
- [ ] Kiem thu tong hop cac lenh sau khi merge

## Son

Phu trach: cac lenh dac biet va bien moi truong.

- [x] `exit`: thoat shell
- [x] `help`: in danh sach lenh
- [x] `date`: hien thi ngay hien tai
- [x] `time`: hien thi gio hien tai
- [x] `dir [path]`: liet ke noi dung thu muc
- [x] `path`: xem bien moi truong `PATH`
- [x] `addpath <dir>`: them thu muc vao `PATH` trong phien shell hien tai
- [ ] Kiem tra lai output tieng Viet tren terminal Windows
- [ ] Sua warning trong `cmd_path()` neu duoc phep sua code

File lien quan:

- `src/son_commands.cpp`
- `include/son_commands.hpp`

## Huy

Phu trach: shell nhan lenh, phan tich lenh va tao tien trinh con de thuc hien.

- [x] Tao `src/huy_commands.cpp`
- [x] Tao `include/huy_commands.hpp`
- [ ] Implement logic that trong `handle_huy_command()`
- [ ] Chay lenh foreground: shell doi tien trinh con ket thuc
- [ ] Chay lenh background voi cu phap ket thuc bang `&`
- [ ] Luu thong tin background process de phan `list/kill/stop/resume` su dung
- [ ] Xu ly loi khi lenh khong ton tai hoac khong tao duoc process
- [x] Cap nhat `Makefile` de build module cua Huy

Yeu cau nghiem thu:

- [ ] `notepad` hoac mot lenh tuong duong chay foreground dung cach
- [ ] `notepad &` hoac mot lenh tuong duong chay background va shell tiep tuc nhan lenh
- [ ] Lenh sai in thong bao loi ro rang, khong lam shell thoat

## Manh

Phu trach: quan ly tien trinh va xu ly tin hieu ngat tu ban phim.

- [x] Tao `src/manh_commands.cpp`
- [x] Tao `include/manh_commands.hpp`
- [ ] Implement logic that trong `handle_manh_command()`
- [ ] `list`: liet ke cac background process dang chay
- [ ] `kill <pid>`: ket thuc mot background process
- [ ] `stop <pid>`: tam dung mot background process
- [ ] `resume <pid>`: tiep tuc mot background process da tam dung
- [ ] Xu ly Ctrl+C de huy foreground process dang chay
- [ ] Don dep handle/process da ket thuc de tranh ro ri tai nguyen
- [x] Cap nhat `Makefile` de build module cua Manh

Yeu cau nghiem thu:

- [ ] `list` chi hien thi process con song
- [ ] `kill <pid>` ket thuc dung process
- [ ] `stop <pid>` tam dung dung process
- [ ] `resume <pid>` tiep tuc dung process
- [ ] Ctrl+C khong lam shell chinh bi thoat ngoai y muon

## Cuong

Phu trach: thuc thi file batch.

- [x] Tao `src/cuong_commands.cpp`
- [x] Tao `include/cuong_commands.hpp`
- [ ] Implement logic that trong `handle_cuong_command()`
- [ ] Nhan dien va thuc thi file `*.bat`
- [ ] Truyen duong dan file batch vao process thuc thi phu hop
- [ ] Xu ly file `.bat` khong ton tai
- [ ] Viet mot vai file `.bat` mau de kiem thu
- [x] Cap nhat `Makefile` de build module cua Cuong

Yeu cau nghiem thu:

- [ ] Chay duoc file `.bat` trong thu muc hien tai
- [ ] Chay duoc file `.bat` bang duong dan tuong doi hoac tuyet doi
- [ ] File `.bat` loi hoac khong ton tai khong lam shell crash

## Tich hop

`src/main.cpp` hien dang dieu phoi lenh theo thu tu:

1. Lenh cua Son: `handle_son_command()`
2. Lenh quan ly tien trinh cua Manh: `kill`, `stop`, `resume`, `list`
3. File `.bat` cua Cuong
4. Cac lenh con lai cua Huy

Checklist khi merge code:

- [x] Them header cua tung module vao `src/main.cpp`
- [x] Xoa cac stub `handle_huy_command()`, `handle_manh_command()`, `handle_cuong_command()` khoi `src/main.cpp`
- [ ] Dam bao moi module tra ve trang thai ro rang sau khi xu ly lenh
- [x] Cap nhat `Makefile` voi tat ca file `.cpp` moi
- [x] Chay `mingw32-make -B`
- [ ] Chay thu cac lenh trong README
- [ ] Cap nhat README neu co thay doi ve lenh hoac cach build

## Goi y cau truc sau khi hoan thien

```text
TinyShell/
├── src/
│   ├── main.cpp
│   ├── son_commands.cpp
│   ├── huy_commands.cpp
│   ├── manh_commands.cpp
│   └── cuong_commands.cpp
├── include/
│   ├── son_commands.hpp
│   ├── huy_commands.hpp
│   ├── manh_commands.hpp
│   └── cuong_commands.hpp
├── docs/
│   └── TODO.md
├── bin/
├── Makefile
├── README.md
└── .gitignore
```
