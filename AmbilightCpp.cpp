#include <iostream>
#include <string>
#include <windows.h>

//custom class
#include "ColorRGBA.h"
#include "Screenshot.h"

//xml
#include "tinyxml2.h"

//serial comm
#include "SerialPort.h"

//opencv
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;


int main()
{
    //hide console
    if (::IsWindowVisible(::GetConsoleWindow())) {
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    }

    //config params
    int scaleFactor = 6;
    int fps = 30;
    bool preview = false;

    int screeny = 0;
    int screenx = 0;
    int width = 1920;
    int height = 1080;


    //read params from config.xml
    tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
    doc->LoadFile("config.xml");
 

    tinyxml2::XMLElement* root = doc->RootElement();
    root->FirstChildElement("scaleFactor")->QueryIntText(&scaleFactor);
    root->FirstChildElement("fps")->QueryIntText(&fps);
    root->FirstChildElement("preview")->QueryBoolText(&preview);

    tinyxml2::XMLElement* screen = root->FirstChildElement("screen");
    screen->FirstChildElement("screeny")->QueryIntText(&screeny);
    screen->FirstChildElement("screenx")->QueryIntText(&screenx);
    screen->FirstChildElement("width")->QueryIntText(&width);
    screen->FirstChildElement("height")->QueryIntText(&height);

    /*delete doc;
    doc = nullptr;
    root = nullptr;
    screen = nullptr;*/


    //apply config
    ColorRGBA* rgb = new ColorRGBA();
    Screenshot* screenshot = new Screenshot(screenx, screeny, width, height);
    
    int delay = 1000 / fps;

    int rheight = height / scaleFactor;
    cv::Size newSize(width / scaleFactor, rheight);
    cv::Size squareSize(rheight, rheight);
    cv::Mat frame;

    //serial comm
    char comPort[] = "\\\\.\\COM3";
    //const char* configComPort = root->FirstChildElement("serialPort")->Attribute("name");

    /*if (configComPort) {
        char* temp = strdup(configComPort);
    }*/

    SerialPort serialport(comPort, CBR_9600);


    if (preview) {
        for (;;) {
            cv::resize(screenshot->captureScreenMat(), frame, newSize, 0, 0);
            rgb->extractFromMat(frame);

            cv::hconcat(frame, cv::Mat(squareSize, CV_8UC4, rgb->toScalar()), frame);
            cv::imshow("preview", frame);

            if (serialport.connected_) {
                serialport.WriteSerialPort(rgb->toJson());      
            }

            if (cv::waitKey(delay) >= 0) break;
        }
    }
    else {
        for (;;) {
            cv::resize(screenshot->captureScreenMat(), frame, newSize, 0, 0);
            rgb->extractFromMat(frame);

            if (serialport.connected_) {
                serialport.WriteSerialPort(rgb->toJson());
            }

            if (cv::waitKey(delay) >= 0) break;
        }
    }

    return 0;
}