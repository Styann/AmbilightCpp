#pragma once
#include <Windows.h>
#include <opencv2/core.hpp>

using namespace cv;

class Screenshot {

private:
    int screenx;
    int screeny;
    int width;
    int height;

    HWND hwnd;
    HDC hwindowDC;
    HDC hwindowCompatibleDC;
    BITMAPINFOHEADER bi;
    HBITMAP hbwindow;

    Mat src;

public:
    Screenshot(int screenx, int screeny, int width, int height);

    ~Screenshot(void);

    Mat captureScreenMat(void);
};