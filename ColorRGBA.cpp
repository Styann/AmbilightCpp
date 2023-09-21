#include "ColorRGBA.h"

ColorRGBA::ColorRGBA(unsigned char r, unsigned char g, unsigned char b, float a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void ColorRGBA::set(unsigned char r, unsigned char g, unsigned char b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

void ColorRGBA::reset() {
    this->set(0, 0, 0);
}

std::string ColorRGBA::toString() {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "r:%d g:%d b:%d ", this->r, this->g, this->b);
    return buffer;
}

char* ColorRGBA::toJson() {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "{ \"ColorRGB\":[%d, %d, %d] }", this->r, this->g, this->b);
    return buffer;
}

cv::Scalar ColorRGBA::toScalar() {
    return cv::Scalar(this->b, this->g, this->r, this->a);
}

void ColorRGBA::extractFromMat(cv::Mat* frame) {
    int r = 0; int g = 0; int b = 0;
    int pixelsSum = 0;
    cv::Point3_<uchar>* pixel;

    for (int y = 0; y < frame->rows; y++) {
        for (int x = 0; x < frame->cols; x++) {
            pixel = frame->ptr<cv::Point3_<uchar> >(y, x);

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

    this->set(r, g, b);
}