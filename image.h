#ifndef RAYTRACER_IMAGE_H
#define RAYTRACER_IMAGE_H

class Image {
public:
    int width;
    int height;
    int* data;

    Image(int width, int height, int *data) : width(width), height(height), data(data) {}

    virtual ~Image() {
        delete[] data;
    }
};

#endif//RAYTRACER_IMAGE_H
