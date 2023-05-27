#include <iostream>
#include <string>
#include <windows.h>

//json
#include <fstream>

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

int scaleFactor = 6;
int fps = 30;
bool video = true;
bool color = false;

int screeny = 0;
int screenx = 0;
int width = 1920;
int height = 1080;

void readJSON() {
    std::ifstream config("config.json")
}

void readXML() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile("parameters.xml");
    tinyxml2::XMLElement* root = doc.RootElement();

    root->FirstChildElement("scaleFactor")->QueryIntText(&scaleFactor);
    root->FirstChildElement("fps")->QueryIntText(&fps);

    tinyxml2::XMLElement* preview = root->FirstChildElement("preview");
    preview->FirstChildElement("video")->QueryBoolText(&video);
    preview->FirstChildElement("color")->QueryBoolText(&color);

    tinyxml2::XMLElement* screen = root->FirstChildElement("screen");
    screen->FirstChildElement("screeny")->QueryIntText(&screeny);
    screen->FirstChildElement("screenx")->QueryIntText(&screenx);
    screen->FirstChildElement("width")->QueryIntText(&width);
    screen->FirstChildElement("height")->QueryIntText(&height);

    cout << screeny;
}

void extractDominantColor(cv::Mat &frame, ColorRGB* rgb) {
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
    readXML();

    ColorRGB* rgb = new ColorRGB();
    Screenshot* screenshot = new Screenshot(screenx, screeny, width, height);
    
    cv::Mat frame;
    cv::Mat resized;

    while (true) {

        frame = screenshot->captureScreenMat();

        cv::resize(frame, resized, cv::Size(frame.cols/6, frame.rows/6), 0, 0);
        extractDominantColor(resized, rgb);
        
        cv::rectangle(resized, cv::Rect(100, 100, 100, 100), rgb->toScalar(), 50, 0, 0);
        
        cv::imshow("capture", resized);

        if (cv::waitKey(1000 / fps) >= 0) {
            break;
        }

    }

    return 0;
}


