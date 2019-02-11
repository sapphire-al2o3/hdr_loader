#ifndef HDR_H
#define HDR_H

#include <stdio.h>
#include <cmath>
#include <iostream>
#include "image.h"

struct RGBFloat {
    float r, g, b;
};

void ReadRLE(unsigned char* channel, FILE* fp, int length) {
    int index = 0;
    while (index < length) {
        unsigned char buf[1];
        fread(buf, 1, 1, fp);
        int n = buf[0];
        if (n > 128) {
            fread(buf, 1, 1, fp);
            for (int k = 0; k < n - 128; k++) {
                channel[index++] = buf[0];
            }
        } else {
            for (int k = 0; k < n; k++) {
                fread(buf, 1, 1, fp);
                channel[index++] = buf[0];
            }
        }
    }
    
}

int LoadHDRImage(const char* fileName, Image<RGBFloat>& image) {

    FILE* fp;
    fopen_s(&fp, fileName, "rb");

    if (fileName == nullptr || fp == nullptr) return 0;

    unsigned char buf[16];

    while (1) {
        fread(buf, 1, 1, fp);
        if (buf[0] == 0x0A) {
            fread(buf, 1, 1, fp);
            if (buf[0] == 0x0A) {
                break;
            }
        }
    }

    int width = 0;
    int height = 0;
    int line;
    fread(buf, 3, 1, fp);
    if (buf[1] == 'Y') {
        while (1) {
            fread(buf, 1, 1, fp);
            if (buf[0] == 0x20) {
                break;
            }
            height = height * 10 + (buf[0] - '0');
        }
        fread(buf, 3, 1, fp);
        while (1) {
            fread(buf, 1, 1, fp);
            if (buf[0] == 0x0A) {
                break;
            }
            width = width * 10 + (buf[0] - '0');
        }
        line = height;
    }

    if (image.lines == nullptr) {
        image.create(width, height);
    }

    unsigned char* r = new unsigned char[width];
    unsigned char* g = new unsigned char[width];
    unsigned char* b = new unsigned char[width];
    unsigned char* e = new unsigned char[width];

    for (int i = 0; i < line; i++) {
        fread(buf, 2, 1, fp);
        if (buf[0] != 0x2 || buf[1] != 0x2) {
            std::cout << "error" << ftell(fp) << std::endl;
            break;
        }
        fread(buf, 2, 1, fp);
        int count = (buf[0] << 8) + buf[1];

        ReadRLE(r, fp, count);
        ReadRLE(g, fp, count);
        ReadRLE(b, fp, count);
        ReadRLE(e, fp, count);

        for (int j = 0; j < count; j++) {
            float p = ldexp(1.0f, e[j] - (128 + 8));
            RGBFloat c = { r[j] * p, g[j] * p, b[j] * p };
            image.lines[i][j] = c;
        }
    }

    delete[] r;
    delete[] g;
    delete[] b;
    delete[] e;

    return 1;
}

#endif