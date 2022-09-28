#include<iostream>
//fmt
#include<fmt/core.h>
#include<fmt/format.h>
#include<fmt/std.h>
#include<fmt/os.h>
#include<fmt/ostream.h>
#include<fmt/color.h>
//windows
#ifdef WIN
    #include<Windows.h>
    #pragma comment(lib,"winmm.lib")
    #define t1 444
#endif

void world_print();
void midi_print();