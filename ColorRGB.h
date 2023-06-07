#pragma once
#include <string>
#include <opencv2/core.hpp>

using namespace std;

class ColorRGB
{
private:
    unsigned char r;
	unsigned char g;
	unsigned char b;
    float opacity;

public:
	ColorRGB(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, float opacity = 1) {
		this->r = r;
		this->g = g;
		this->b = b;
        this->opacity = opacity;
	}

    void set(unsigned char r, unsigned char g, unsigned char b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void reset() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }

    std::string toString() {
        char buff[100];
        snprintf(buff, sizeof(buff), "r:%d g:%d b:%d ", this->r, this->g, this->b);
        return buff;
    }

    cv::Scalar toScalar() {
        return cv::Scalar(this->b, this->g, this->r, this->opacity);
    }

};

