#include<algorithm>
#include<thread>
//opencv
#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
//导入自己写的头文件
#include"utils.h"
#include"get_screen.h"

void thread_print(){
    helloworld_print();
    midi_print();
}

void thread_screen(){

}


int main(){
    std::thread t1_p(thread_print);
    std::thread t2_s(thread_screen);
    t1_p.join();
    t2_s.join();
    std::cout << "hello world!" << std::endl;
    return 0;
}

