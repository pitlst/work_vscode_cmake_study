#include<algorithm>
#include<string>
#include<thread>
//opencv
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
//fmt
#include<fmt/core.h>
#include<fmt/format.h>
#include<fmt/std.h>
#include<fmt/os.h>
#include<fmt/ostream.h>
#include<fmt/color.h>
//导入自己写的头文件
#include"utils.h"
#include"get_screen.h"


void thread_print(){
    helloworld_print();
    midi_print();
}

void thread_screen(){
    Screenshot screenshot;
    cv::Mat img = screenshot.getScreenshot();
    cv::Mat img_ = screenshot.getScreenshot(1040, 132, 800, 880);
    cv::imwrite("screenshot.jpg", img);
    cv::imwrite("screenshot_part.jpg", img_);
}


int main(){
    std::thread t1_p(thread_print);
    std::thread t2_s(thread_screen);
    t1_p.join();
    t2_s.join();
    fmt::print("thread finish\n");
    return 0;
}

