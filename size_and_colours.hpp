#pragma once
#include<ostream>
#include"low_lvl.h"

struct console_colour
{
    bool fore;
    enum CONCOLS col;
};

extern console_colour black_background;
extern console_colour red_background;
extern console_colour green_background;
extern console_colour yellow_background;
extern console_colour blue_background;
extern console_colour magenta_background;
extern console_colour cyan_background;
extern console_colour white_background;

extern console_colour black_foreground;
extern console_colour red_foreground;
extern console_colour green_foreground;
extern console_colour yellow_foreground;
extern console_colour blue_foreground;
extern console_colour magenta_foreground;
extern console_colour cyan_foreground;
extern console_colour white_foreground;

extern console_colour reset_console;

std::pair<int, int>get_console_size_cpp();
std::ostream& operator<<(std::ostream& os, const console_colour& col);