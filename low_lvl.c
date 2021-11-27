#include<stdio.h>
#include<windows.h>

void enable_colours(void)
{
    DWORD cm;
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hout, &cm);
    cm |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hout, cm);
}

void get_console_size(int* width, int* height)
{
    CONSOLE_SCREEN_BUFFER_INFO ci;
    HANDLE ho = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(ho, &ci);
    *width = ci.srWindow.Right - ci.srWindow.Left + 1;
    *height = ci.srWindow.Bottom - ci.srWindow.Top + 1;
}

void concol_set_foreground(enum CONCOLS colour)
{
    int msg = colour << 24 | 0x335b1b;
    fwrite(&msg, sizeof(msg), 1, stdout);
    fputc('m', stdout);
    fflush(stdout);
}

void concol_set_background(enum CONCOLS colour)
{
    int msg = colour << 24 | 0x345b1b;
    fwrite(&msg, sizeof(msg), 1, stdout);
    fputc('m', stdout);
    fflush(stdout);
}

void concol_reset(void)
{
    int msg = 0x6d305b1b;
    fwrite(&msg, sizeof(msg), 1, stdout);
}