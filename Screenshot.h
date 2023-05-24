#pragma once
#include <Windows.h>
#include <opencv2/core.hpp>

using namespace cv;

class Screenshot
{
private:
    int screenx;
    int screeny;
    int width;
    int height;
    HWND hwnd;

public:
    Screenshot(int screenx, int screeny, int width, int height) {
        this->screenx = screenx;
        this->screeny = screeny;
        this->width = width;
        this->height = height;
        this->hwnd = GetDesktopWindow();
    }


    BITMAPINFOHEADER createBitmapHeader(int width, int height){

        BITMAPINFOHEADER bi;
        
        // create a bitmap
        bi.biSize = sizeof(BITMAPINFOHEADER);

        bi.biWidth = width;

        bi.biHeight = -height;  //this is the line that makes it draw upside down or not

        bi.biPlanes = 1;

        bi.biBitCount = 32;

        bi.biCompression = BI_RGB;

        bi.biSizeImage = 0;

        bi.biXPelsPerMeter = 0;

        bi.biYPelsPerMeter = 0;

        bi.biClrUsed = 0;

        bi.biClrImportant = 0;


        return bi;
    }

    Mat captureScreenMat()
    {

        Mat src;


        // get handles to a device context (DC)

        HDC hwindowDC = GetDC(this->hwnd);

        HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);

        SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);


        // define scale, height and width

        /*int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
        int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
        int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);*/


        // create mat object

        src.create(this->height, this->width, CV_8UC4);


        // create a bitmap

        HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, this->width, this->height);

        BITMAPINFOHEADER bi = createBitmapHeader(this->width, this->height);


        // use the previously created device context with the bitmap

        SelectObject(hwindowCompatibleDC, hbwindow);


        // copy from the window device context to the bitmap device context

        StretchBlt(hwindowCompatibleDC, 0, 0, this->width, this->height, hwindowDC, this->screenx, this->screeny, this->width, this->height, SRCCOPY);

        GetDIBits(hwindowCompatibleDC, hbwindow, 0, this->height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS); //copy from hwindowCompatibleDC to hbwindow


        // avoid memory leak

        DeleteObject(hbwindow);

        DeleteDC(hwindowCompatibleDC);

        ReleaseDC(this->hwnd, hwindowDC);


        return src;

    }

};

