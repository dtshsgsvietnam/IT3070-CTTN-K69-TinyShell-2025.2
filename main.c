/*
 * TinyShell - Project 1 (myShell)
 * File: main.c  –  Vòng lặp chính của shell
 *
 * Tích hợp phần của Sơn (son_commands.c).
 * Các phần của Huy, Mạnh, Cường được khai báo stub ở đây để
 * main.c biên dịch được độc lập trong khi chờ merge.
 *
 * Biên dịch:
 *   gcc -Wall -o myShell main.c son_commands.c -lkernel32
 *
 * Chạy:
 *   myShell.exe
 */

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "son_commands.h"

/* ─── Forward-declarations cho phần của các thành viên khác ─── */
/* Huy: foreground/background execution */
int  handle_huy_command(const char *cmd_line);   /* stub */

/* Mạnh: kill, stop, resume, list + Ctrl+C */
int  handle_manh_command(const char *cmd_line);  /* stub */

/* Cường: thực thi file .bat */
int  handle_cuong_command(const char *cmd_line); /* stub */

/* ─── Stub implementations (xóa khi merge code thật) ────────── */
int handle_huy_command(const char *cmd_line) {
    (void)cmd_line;
    printf("  [Huy] foreground/background execution – chưa implement\n");
    return 1;
}
int handle_manh_command(const char *cmd_line) {
    (void)cmd_line;
    printf("  [Mạnh] process management – chưa implement\n");
    return 1;
}
int handle_cuong_command(const char *cmd_line) {
    (void)cmd_line;
    printf("  [Cường] .bat execution – chưa implement\n");
    return 1;
}

/* ─── Hàm kiểm tra file .bat ─────────────────────────────────── */
static int is_bat_file(const char *cmd) {
    size_t len = strlen(cmd);
    if (len < 4) return 0;
    const char *ext = cmd + len - 4;
    return (_stricmp(ext, ".bat") == 0);
}

/* ─── Hàm kiểm tra lệnh quản lý tiến trình của Mạnh ─────────── */
static int is_manh_command(const char *cmd) {
    char buf[64];
    strncpy(buf, cmd, 63); buf[63] = '\0';
    for (int i = 0; buf[i]; i++)
        if (buf[i] >= 'A' && buf[i] <= 'Z') buf[i] += 32;
    return (strncmp(buf, "kill", 4)   == 0 ||
            strncmp(buf, "stop", 4)   == 0 ||
            strncmp(buf, "resume", 6) == 0 ||
            strncmp(buf, "list", 4)   == 0);
}

/* ════════════════════════════════════════════════════════════════
 * main – vòng lặp REPL của myShell
 * ════════════════════════════════════════════════════════════════ */
int main(void)
{
    char cmd_line[4096];
    int  should_exit = 0;

    /* Tiêu đề */
    printf("\n");
    printf("  ╔══════════════════════════════════════════╗\n");
    printf("  ║              myShell v1.0                ║\n");
    printf("  ║   Gõ 'help' để xem danh sách lệnh       ║\n");
    printf("  ╚══════════════════════════════════════════╝\n\n");

    while (!should_exit) {
        /* In prompt */
        char cwd[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, cwd);
        printf("myShell\\%s>", cwd);
        fflush(stdout);

        /* Đọc lệnh */
        if (fgets(cmd_line, sizeof(cmd_line), stdin) == NULL)
            break;

        /* Bỏ newline cuối */
        cmd_line[strcspn(cmd_line, "\r\n")] = '\0';

        /* Bỏ qua dòng trống */
        char *trimmed = cmd_line;
        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
        if (strlen(trimmed) == 0) continue;

        /* ── Ưu tiên dispatcher: lệnh của Sơn trước ── */
        if (handle_son_command(trimmed, &should_exit))
            continue;

        /* ── Lệnh quản lý tiến trình của Mạnh ── */
        if (is_manh_command(trimmed)) {
            handle_manh_command(trimmed);
            continue;
        }

        /* ── File .bat → Cường ── */
        if (is_bat_file(trimmed)) {
            handle_cuong_command(trimmed);
            continue;
        }

        /* ── Tất cả lệnh còn lại → Huy (fore/background) ── */
        handle_huy_command(trimmed);
    }

    return 0;
}
