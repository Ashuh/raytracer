#ifndef RAYTRACER_IMAGE_H
#define RAYTRACER_IMAGE_H

#include <chrono>

class Image {
public:
    int width;
    int height;
    int samples;
    std::chrono::milliseconds cumulativeRenderTime;
    int *data;

    Image(int width, int height, int samples, int *data,
          std::chrono::milliseconds cumulativeRenderTime) : width(width), height(height),
                                                  samples(samples), data(data),
                                                  cumulativeRenderTime(cumulativeRenderTime) {}

    virtual ~Image() {
        delete[] data;
    }
};

#endif//RAYTRACER_IMAGE_H
