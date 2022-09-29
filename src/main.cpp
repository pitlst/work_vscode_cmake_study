#include <iostream>
#include <thread>
#include <Windows.h>
//导入自己写的头文件
#include "utils.hpp"
#include "midi.hpp"
#include "Screen.hpp"

using namespace std;

void thread_midi()
{
    midi_print_1();
}

void thread_getbmp()
{
    HBITMAP hBmp;
    double magnification;
    magnification = get_windows_magnification();
    hBmp = GetCaptureBmp(magnification);
    SaveBitmapToFile(hBmp, "temp_screen.bmp");
}

void thread_process()
{

}

int main()
{
    thread t1_midi(thread_midi);
    thread t2_bmp(thread_getbmp);
    thread t3_pr(thread_process);
    t1_midi.join();
    t2_bmp.join();
    t3_pr.join();
    cout << "程序运行完成" << endl;
    return 0;
}
