#include <iostream>
#include "hdr.h"
#include "bmp.h"

int main(int argc, char *argv[]) {
    Image<RGBFloat> img;

    LoadHDRImage(argv[1], img);

    std::cout << "width: " << img.width << std::endl;
    std::cout << "height: " << img.height << std::endl;

    double max = 0;
    double min = 1000;
    for(int i = 0; i < img.height * img.width; i++) {
        RGBFloat c = img.lines[0][i];
        if (c.r > max) max = c.r;
        if (c.g > max) max = c.g;
        if (c.b > max) max = c.b;
        if (c.r < min) min = c.r;
        if (c.g < min) min = c.g;
        if (c.b < min) min = c.b;
    }

    float d = max - min;
    Image<BGR> dst(img.width, img.height);

    // HDR to LDR
    for(int i = 0; i < img.height * img.width; i++) {
        RGBFloat c = img.lines[0][i];
        c.r = (c.r - min) / d;
        c.g = (c.g - min) / d;
        c.b = (c.b - min) / d;

        BGR bgr;
        bgr.r = c.r * 255;
        bgr.g = c.g * 255;
        bgr.b = c.b * 255;
        dst.lines[0][i] = bgr;
    }

    SaveBitmap("hdr_test.bmp", dst);
    return 0;
}