#include "Screenshot.h"

Screenshot::Screenshot(int screenx, int screeny, int width, int height) {
    this->screenx = screenx;
    this->screeny = screeny;
    this->width = width;
    this->height = height;
    this->hwnd = GetDesktopWindow();
}


BITMAPINFOHEADER Screenshot::createBitmapHeader() {
    this->bi.biSize = sizeof(BITMAPINFOHEADER);
    this->bi.biWidth = this->width;
    this->bi.biHeight = -this->height;  //this is the line that makes it draw upside down or not
    this->bi.biPlanes = 1;
    this->bi.biBitCount = 32;
    this->bi.biCompression = BI_RGB;
    this->bi.biSizeImage = 0;
    this->bi.biXPelsPerMeter = 0;
    this->bi.biYPelsPerMeter = 0;
    this->bi.biClrUsed = 0;
    this->bi.biClrImportant = 0;

    return bi;
}

Mat Screenshot::captureScreenMat()
{

    Mat src;


    // get handles to a device context (DC)

    this->hwindowDC = GetDC(this->hwnd);

    this->hwindowCompatibleDC = CreateCompatibleDC(this->hwindowDC);

    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);


    // define scale, height and width

    /*int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);*/


    // create mat object

    src.create(this->height, this->width, CV_8UC4);


    // create a bitmap

    this->hbwindow = CreateCompatibleBitmap(hwindowDC, this->width, this->height);

    this->createBitmapHeader();


    // use the previously created device context with the bitmap

    SelectObject(hwindowCompatibleDC, hbwindow);


    // copy from the window device context to the bitmap device context

    StretchBlt(hwindowCompatibleDC, 0, 0, this->width, this->height, hwindowDC, this->screenx, this->screeny, this->width, this->height, SRCCOPY);

    GetDIBits(hwindowCompatibleDC, hbwindow, 0, this->height, src.data, (BITMAPINFO*)&this->bi, DIB_RGB_COLORS); //copy from hwindowCompatibleDC to hbwindow


    // avoid memory leak

    /*DeleteObject(hbwindow);

    DeleteDC(hwindowCompatibleDC);

    ReleaseDC(this->hwnd, hwindowDC);*/


    return src;

}
