#include <iostream>
#include <thread>
#include <Windows.h>
//导入自己写的头文件
#include "utils.hpp"
#include "Screen.hpp"

using namespace std;

void thread_midi()
{
    midi_print();
}

void thread_getbmp()
{
    HBITMAP hBmp;
    double magnification;
    magnification = get_windows_magnification();
    hBmp = GetCaptureBmp(magnification);
    SaveBitmapToFile(hBmp, "temp_screen.bmp");
}

int main()
{
    thread t1_midi(thread_midi);
    thread t2_bmp(thread_getbmp);
    cout << "hello world" << endl;
    t1_midi.join();
    t2_bmp.join();
    return 0;
}
