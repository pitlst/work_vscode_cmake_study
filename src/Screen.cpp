#include <stdio.h>
#include <assert.h>
#include <windows.h>

double get_windows_magnification()
{
    // 获取窗口当前显示的监视器
    // 使用桌面的句柄.
    HWND hWnd = GetDesktopWindow();
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    // 获取监视器逻辑宽度与高度
    MONITORINFOEX miex;
    miex.cbSize = sizeof(miex);
    GetMonitorInfo(hMonitor, &miex);
    int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
    int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

    // 获取监视器物理宽度与高度
    DEVMODE dm;
    dm.dmSize = sizeof(dm);
    dm.dmDriverExtra = 0;
    EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
    int cxPhysical = dm.dmPelsWidth;
    int cyPhysical = dm.dmPelsHeight;

    // 缩放比例计算  实际上使用任何一个即可
    double horzScale = ((double)cxPhysical / (double)cxLogical);
    double vertScale = ((double)cyPhysical / (double)cyLogical);
    assert(horzScale == vertScale); // 宽或高这个缩放值应该是相等的
    return horzScale;
}

/************************************************************************/
/* hBitmap    为刚才的屏幕位图句柄
/* lpFileName 为需要保存的位图文件名
/************************************************************************/
int SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName)
{
    HDC hDC;                 //设备描述表
    int iBits;               //当前显示分辨率下每个像素所占字节数
    WORD wBitCount;          //位图中每个像素所占字节数
    DWORD dwPaletteSize = 0; //定义调色板大小
    DWORD dwBmBitsSize;      //位图中像素字节大小
    DWORD dwDIBSize;         //位图文件大小
    DWORD dwWritten;         //写入文件字节数
    BITMAP Bitmap;           //位图结构
    BITMAPFILEHEADER bmfHdr; //位图属性结构
    BITMAPINFOHEADER bi;     //位图文件头结构
    LPBITMAPINFOHEADER lpbi; //位图信息头结构     指向位图信息头结构
    HANDLE fh;               //定义文件句柄
    HANDLE hDib;             //分配内存句柄
    HANDLE hPal;             //分配内存句柄
    HANDLE hOldPal = NULL;   //调色板句柄

    //计算位图文件每个像素所占字节数

    hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    DeleteDC(hDC);

    if (iBits <= 1)
        wBitCount = 1;
    else if (iBits <= 4)
        wBitCount = 4;
    else if (iBits <= 8)
        wBitCount = 8;
    else if (iBits <= 24)
        wBitCount = 24;
    else if (iBits <= 32)
        wBitCount = 24;

    //计算调色板大小
    if (wBitCount <= 8)
        dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);

    //设置位图信息头结构
    GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = Bitmap.bmWidth;
    bi.biHeight = Bitmap.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
    dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
    //为位图内容分配内存
    hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    if (lpbi == NULL)
    {
        return 0;
    }

    *lpbi = bi;
    // 处理调色板
    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal)
    {
        hDC = GetDC(NULL);
        hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }
    // 获取该调色板下新的像素值
    GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
              (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
              (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
    //恢复调色板
    if (hOldPal)
    {
        SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
        RealizePalette(hDC);
        ReleaseDC(NULL, hDC);
    }
    //创建位图文件
    fh = CreateFile(lpFileName, GENERIC_WRITE,
                    0, NULL, CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (fh == INVALID_HANDLE_VALUE)
        return FALSE;

    // 设置位图文件头
    bmfHdr.bfType = 0x4D42; // "BM"
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

    // 写入位图文件头
    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

    // 写入位图文件其余内容
    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

    //清除
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    CloseHandle(fh);

    return 1;
}

HBITMAP GetCaptureBmp(double magnification)
{
    HDC hDC;
    HDC MemDC;
    BYTE *Data;
    HBITMAP hBmp;
    BITMAPINFO bi;

    memset(&bi, 0, sizeof(bi));
    bi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bi.bmiHeader.biWidth = GetSystemMetrics(SM_CXSCREEN) * magnification;
    bi.bmiHeader.biHeight = GetSystemMetrics(SM_CYSCREEN) * magnification;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 24;

    hDC = GetDC(NULL);
    MemDC = CreateCompatibleDC(hDC);
    hBmp = CreateDIBSection(MemDC, &bi, DIB_RGB_COLORS, (void **)&Data, NULL, 0);
    SelectObject(MemDC, hBmp);
    BitBlt(MemDC, 0, 0, bi.bmiHeader.biWidth, bi.bmiHeader.biHeight, hDC, 0, 0, SRCCOPY);
    ReleaseDC(NULL, hDC);
    DeleteDC(MemDC);
    return hBmp;
}
