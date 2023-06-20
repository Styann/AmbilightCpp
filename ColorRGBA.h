#pragma once
#include <string>
#include <opencv2/core.hpp>

using namespace std;

class ColorRGBA {

private:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    float a;

public:
    ColorRGBA(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, float a = 1);
    void set(unsigned char r, unsigned char g, unsigned char b);
    void reset();
    std::string toString();
    char* toJson();
    cv::Scalar toScalar();
    void extractFromMat(cv::Mat& frame);
};