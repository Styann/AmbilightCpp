#include <iostream>
#include <string>
#include <windows.h>

//custom class
#include "ColorRGB.h"
#include "Screenshot.h"

//xml
#include "tinyxml2.h"

//opencv
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


void extractDominantColor(cv::Mat& frame, ColorRGB* rgb) {
    int r = 0; int g = 0; int b = 0;
    int pixelsSum = 0;
    cv::Point3_<uchar>* pixel;

    for (int y = 0; y < frame.rows; y++) {
        for (int x = 0; x < frame.cols; x++) {
            pixel = frame.ptr<cv::Point3_<uchar> >(y, x);

            if ((pixel->z * 0.2126 + pixel->y * 0.7152 + pixel->x * 0.0722) >= 127) {
                pixelsSum++;
                r += pixel->z;
                g += pixel->y;
                b += pixel->x;
            }
        }
    }

    if (pixelsSum > 0) {
        r /= pixelsSum;
        g /= pixelsSum;
        b /= pixelsSum;
    }

    rgb->set(r, g, b);
}

int main()
{
    //::ShowWindow(::GetConsoleWindow(), SW_HIDE);

    //config params
    int scaleFactor = 6;
    int fps = 30;
    bool preview = false;

    int screeny = 0;
    int screenx = 0;
    int width = 1920;
    int height = 1080;


    //read params from config.xml
    tinyxml2::XMLDocument doc;
    doc.LoadFile("config.xml");
    tinyxml2::XMLElement* root = doc.RootElement();

    root->FirstChildElement("scaleFactor")->QueryIntText(&scaleFactor);
    root->FirstChildElement("fps")->QueryIntText(&fps);
    root->FirstChildElement("preview")->QueryBoolText(&preview);

    tinyxml2::XMLElement* screen = root->FirstChildElement("screen");
    screen->FirstChildElement("screeny")->QueryIntText(&screeny);
    screen->FirstChildElement("screenx")->QueryIntText(&screenx);
    screen->FirstChildElement("width")->QueryIntText(&width);
    screen->FirstChildElement("height")->QueryIntText(&height);

    //apply config
    ColorRGB* rgb = new ColorRGB();
    Screenshot* screenshot = new Screenshot(screenx, screeny, width, height);
    
    int delay = 1000 / fps;

    int rheight = height / scaleFactor;
    cv::Size newSize(width / scaleFactor, rheight);
    cv::Size squareSize(rheight, rheight);
    cv::Mat frame;



    if (preview) {
        while (cv::waitKey(delay) < 0) {
            cv::resize(screenshot->captureScreenMat(), frame, newSize, 0, 0);
            extractDominantColor(frame, rgb);

            cv::hconcat(frame, cv::Mat(squareSize, CV_8UC4, rgb->toScalar()), frame);
            cv::imshow("preview", frame);
        }
    }
    else {
        while (cv::waitKey(delay) < 0) {
            cv::resize(screenshot->captureScreenMat(), frame, newSize, 0, 0);
            extractDominantColor(frame, rgb);
        }
    }

    return 0;
}