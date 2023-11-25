#include "Screenshot.h"

Screenshot::Screenshot(int screenx, int screeny, int width, int height) {
    this->screenx = screenx;
    this->screeny = screeny;
    this->width = width;
    this->height = height;
    this->hwnd = GetDesktopWindow();

    // create mat object
    src.create(this->height, this->width, CV_8UC4);

    // get handles to a device context (DC)
    this->hwindowDC = GetDC(this->hwnd);

    this->hwindowCompatibleDC = CreateCompatibleDC(this->hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    // create a bitmap
    this->hbwindow = CreateCompatibleBitmap(hwindowDC, this->width, this->height);

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

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
}

Screenshot::~Screenshot(void) {
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(this->hwnd, hwindowDC);
}

Mat Screenshot::captureScreenMat(void) {
    // copy from the window device context to the bitmap device context
    StretchBlt(this->hwindowCompatibleDC, 0, 0, this->width, this->height, hwindowDC, this->screenx, this->screeny, this->width, this->height, SRCCOPY);
    //copy from hwindowCompatibleDC to hbwindow
    GetDIBits(this->hwindowCompatibleDC, this->hbwindow, 0, this->height, this->src.data, (BITMAPINFO*)&this->bi, DIB_RGB_COLORS); 

    return src;
}
