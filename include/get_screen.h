#pragma once  //保证该文件不被include多次
#include<Windows.h>
#include<opencv2/opencv.hpp>

using namespace cv;

class Screenshot
{
public:
    Screenshot();
    double static getZoom();
    cv::Mat getScreenshot();
    cv::Mat getScreenshot(int x, int y, int width, int height);

private:
    int m_width;
    int m_height;
    HDC m_screenDC;
    HDC m_compatibleDC;
    HBITMAP m_hBitmap;
    LPVOID m_screenshotData = nullptr;
};