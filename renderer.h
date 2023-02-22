#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "image.h"
#include <atomic>
#include <mutex>

class Renderer {
public:
    Renderer(int imageWidth, int imageHeight, int samplesPerPixel, int maxDepth) : imageWidth(imageWidth),
                                                                                   imageHeight(imageHeight),
                                                                                   samplesPerPixel(samplesPerPixel),
                                                                                   maxDepth(maxDepth) {}

    std::shared_ptr<Image> render(const Camera &camera, const HittableList &scene) {
        int idx = 0;
        int *data = new int[imageWidth * imageHeight];

        for (int j = imageHeight - 1; j >= 0; --j) {
            auto v = (j + randomDouble()) / (imageHeight - 1);
            for (int i = 0; i < imageWidth; ++i) {
                auto u = (i + randomDouble()) / (imageWidth - 1);
                Color color = pixelColor(scene, camera, u, v);
                data[idx++] = toInt(color);
            }
        }
        std::shared_ptr<Image> img = std::make_shared<Image>(imageWidth, imageHeight, data);
        return img;
    }

    void setImageWidth(int width) {
        imageWidth = width;
    }

    void setImageHeight(int height) {
        imageHeight = height;
    }

    void setSamplesPerPixel(int samples) {
        samplesPerPixel = samples;
    }

    void setMaxDepth(int depth) {
        maxDepth = depth;
    }

    int getImageWidth() const {
        return imageWidth;
    }

    int getImageHeight() const {
        return imageHeight;
    }

    int getSamplesPerPixel() const {
        return samplesPerPixel;
    }

    int getMaxDepth() const {
        return maxDepth;
    }

private:
    std::atomic_int imageWidth;
    std::atomic_int imageHeight;
    std::atomic_int samplesPerPixel;
    std::atomic_int maxDepth;
    mutable std::mutex m;

    Color rayColor(const Ray &r, const Hittable &scene, int depth) {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0) {
            return {0, 0, 0};
        }

        if (auto rec = scene.hit(r, 0.001, std::numeric_limits<double>::infinity())) {
            if (auto scattered = rec->material->scatter(r, *rec)) {
                return rec->material->getAlbedo() * rayColor(*scattered, scene, depth - 1);
            }
            return {0, 0, 0};
        }
        Vec3 unitDirection = unitVector(r.direction());
        auto t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    }

    Color pixelColor(const HittableList &scene, const Camera &camera, double u, double v) {
        Color pixelColor(0, 0, 0);
        for (int s = 0; s < samplesPerPixel; ++s) {
            Ray r = camera.getRay(u, v);
            pixelColor += rayColor(r, scene, maxDepth);
        }

        pixelColor /= samplesPerPixel;
        return pixelColor;
    }
};

#endif//RAYTRACER_RENDERER_H
