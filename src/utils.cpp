#include "utils.hpp"
#include <iostream>



cv::Mat bmp_to_mat() {
    cv::Mat bmp = cv::imread("temp_screen.bmp", cv::IMREAD_UNCHANGED);
    cv::imwrite("my.jpg", bmp);  //保存图片
    return bmp;
}
