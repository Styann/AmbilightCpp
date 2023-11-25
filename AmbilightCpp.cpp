#include <iostream>
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

    //config parameters
    int fps = 24;
    bool preview = false;

    int screeny = 0;
    int screenx = 0;
    int width = 1920;
    int height = 1080;

    const char* portName;

    //read params from config.xml
    tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
    doc->LoadFile("config.xml");

    tinyxml2::XMLElement* root = doc->RootElement();

    root->FirstChildElement("fps")->QueryIntText(&fps);
    root->FirstChildElement("preview")->QueryBoolText(&preview);

    tinyxml2::XMLElement* screen = root->FirstChildElement("screen");
    screen->QueryIntAttribute("screeny", &screeny);
    screen->QueryIntAttribute("screenx", &screenx);
    screen->QueryIntAttribute("width", &width);
    screen->QueryIntAttribute("height", &height);

    root->FirstChildElement("serialPort")->QueryAttribute("name", &portName);
    delete doc, root, screen;


    //apply config
    ColorRGBA* rgb = new ColorRGBA();
    Screenshot* screenshot = new Screenshot(screenx, screeny, width, height);
    
    int delay = 1000 / fps;

    cv::Size squareSize(300, 300);
    cv::Mat frame;

    SerialPort *serialport = new SerialPort((char*)&portName, CBR_9600);


    for(;;) {
        frame = screenshot->captureScreenMat();
        rgb->extractFromMat(&frame);

        if (preview) {
            cv::imshow("preview", cv::Mat(squareSize, CV_8UC4, rgb->toScalar()));
        }

        if (serialport->connected_) {
            serialport->WriteSerialPort(rgb->toJson());      
        }

        cv::waitKey(delay);
    }
    
    return 0;
}