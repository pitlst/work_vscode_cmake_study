#include <iostream>
#include <thread>
//导入自己写的头文件
#include "utils.hpp"
#include "Screen.hpp"

void thread_print(){
    helloworld_print();
    midi_print();
}


int main(){
    HBITMAP   hBmp;
    hBmp = GetCaptureBmp();
    SaveBitmapToFile(hBmp, "c:\\11.bmp");
    std::cout << "hello world" << std::endl;
    thread_print();
    return 0;
}
