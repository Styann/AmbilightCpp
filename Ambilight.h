#pragma once

#include <windows.h>
#include <stdio.h>
#include <iostream>

class Ambilight
{
private:
	BITMAP screenShot;

	int scaleFactor;
	int r, g, b;
	int lightPixelSum;
	COLORREF* pixel;
	
	BITMAP HbitmapBITMAPToBitmap(HBITMAP& hbitmap) {
		BITMAP bitmap;
		GetObject(hbitmap, sizeof(bitmap), &bitmap);
		return bitmap;
	}

public:
	BITMAP TakeScreenShot() {
		HDC hScreenDC = GetDC(nullptr); // CreateDC("DISPLAY",nullptr,nullptr,nullptr);
		HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
		int width = GetDeviceCaps(hScreenDC, HORZRES);
		int height = GetDeviceCaps(hScreenDC, VERTRES);
		HBITMAP hbitmap = CreateCompatibleBitmap(hScreenDC, width, height);
		HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hbitmap));
		BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
		hbitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));
		DeleteDC(hMemoryDC);
		DeleteDC(hScreenDC);

		BITMAP bitmap = this->HbitmapBITMAPToBitmap(hbitmap);
		return bitmap;
	}



	void SetScreenShot(BITMAP &bitmap) {
		this->screenShot = bitmap;
	}

	void ResizeBitmap(BITMAP& bitmap, int scaleFactor) {
		return;
	}

	void GetAverageColorFromBitmap(BITMAP &bitmap) {
		this->r = 0;
		this->g = 0;
		this->b = 0;
		this->lightPixelSum = 0;
		

		for (int x = 0; x < bitmap.bmWidth; x++) {
			for (int y = 0; y < bitmap.bmHeight; y++) {
				std::cout << "1";
			}
		}
	}
};

