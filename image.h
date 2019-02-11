#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>

template <typename T>
class Image
{
public:
    T** lines;
    int width;
    int height;

    Image() : width(0), height(0), lines(nullptr) {}

    Image(int _width, int _height)
    {
        create(_width, _height);
    }

    ~Image() {
        delete[] lines[0];
        delete[] lines;
    }

    void create(int _width, int _height)
    {
        width = _width;
        height = _height;

        lines = new T*[height];
        lines[0] = new T[width * height];
        for(int i = 1; i < height; i++) {
            lines[i] = &lines[0][width * i];
        }
    }
};

#endif
