#include"utils.h"

void world_print(){
    fmt::print("Hello, world!\n");
    fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold,"Hello, {}!\n", "world");
    fmt::print(fg(fmt::color::floral_white) | bg(fmt::color::slate_gray) | fmt::emphasis::underline, "Hello, {}!\n", "мир");
    fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic,"Hello, {}!\n", "世界");
}