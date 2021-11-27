#pragma once

enum CONCOLS
{
    RESET = 0, BLACK = 48, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
};

#ifdef __cplusplus
extern"C"
{
#endif
    void enable_colours(void);
    void get_console_size(int* width, int* height);
    void concol_set_foreground(enum CONCOLS colour);
    void concol_set_background(enum CONCOLS colour);
    void concol_reset(void);
#ifdef __cplusplus
}
#endif