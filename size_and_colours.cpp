#include<iostream>
#include"size_and_colours.hpp"

console_colour black_background{ false, BLACK };
console_colour red_background{ false, RED };
console_colour green_background{ false, GREEN };
console_colour yellow_background{ false, YELLOW };
console_colour blue_background{ false, BLUE };
console_colour magenta_background{ false, MAGENTA };
console_colour cyan_background{ false, CYAN };
console_colour white_background{ false, WHITE };

console_colour black_foreground{ true, BLACK };
console_colour red_foreground{ true, RED };
console_colour green_foreground{ true, GREEN };
console_colour yellow_foreground{ true, YELLOW };
console_colour blue_foreground{ true, BLUE };
console_colour magenta_foreground{ true, MAGENTA };
console_colour cyan_foreground{ true, CYAN };
console_colour white_foreground{ true, WHITE };

console_colour reset_console{ false, RESET };

std::pair<int, int>get_console_size_cpp()
{
    std::pair<int, int>sz;
    get_console_size(&sz.first, &sz.second);
    return sz;
}

std::ostream& operator<<(std::ostream& os, const console_colour& col)
{
    if (&os != &std::cout)
        return os;
    if (col.col)
    {
        if (col.fore)
            concol_set_foreground(col.col);
        else
            concol_set_background(col.col);
    }
    else
        concol_reset();
    return os;
}