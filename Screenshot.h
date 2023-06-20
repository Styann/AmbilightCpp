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

public:
    Screenshot(int screenx, int screeny, int width, int height);

    BITMAPINFOHEADER createBitmapHeader(int width, int height);

    Mat captureScreenMat();
};