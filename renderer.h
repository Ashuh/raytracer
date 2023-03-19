#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

#include "camera.h"
#include "color.h"
#include "gui.h"
#include "hittable_list.h"
#include "image.h"
#include <atomic>
#include <execution>
#include <mutex>

class Renderer {
public:
    Renderer(int imageWidth, int imageHeight, int maxDepth) : imageWidth(imageWidth),
                                                              imageHeight(imageHeight),
                                                              maxDepth(maxDepth) {
        cumulativeData.resize(imageWidth * imageHeight);
    }

    std::shared_ptr<Image> render(const Camera &camera, const HittableList &scene) {
        isRendering = true;
        const int numPixels = imageWidth * imageHeight;
        int *data = new int[numPixels];

        std::vector<int> indices(numPixels);
        std::generate(indices.begin(), indices.end(), [n = 0]() mutable { return n++; });

        auto start = std::chrono::high_resolution_clock::now();
        std::find_if(
                std::execution::par,
                indices.begin(),
                indices.end(),
                [this, &scene, &camera, &data, numPixels](auto &&i) {
                    if (isInterrupted) {
                        return true;
                    }

                    int row = static_cast<double>((numPixels - 1) - i) / imageWidth;
                    int col = i % imageWidth;
                    auto v = (row + randomDouble()) / (imageHeight - 1);
                    auto u = (col + randomDouble()) / (imageWidth - 1);
                    Color color = pixelColor(scene, camera, u, v);
                    cumulativeData[i] += color;
                    data[i] = toInt(cumulativeData[i] / (samplesAccumulated + 1));
                    return false;
                });
        auto end = std::chrono::high_resolution_clock::now();

        if (isInterrupted) {
            isInterrupted = false;
            isRendering = false;
            return nullptr;
        }

        samplesAccumulated++;
        auto durationMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cumulativeRenderTimeMillis += durationMillis;
        std::shared_ptr<Image> img = std::make_shared<Image>(imageWidth, imageHeight, samplesAccumulated, data, cumulativeRenderTimeMillis);
        isRendering = false;
        return img;
    }

    void setImageWidth(int width) {
        imageWidth = width;
    }

    void setImageHeight(int height) {
        imageHeight = height;
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

    int getMaxDepth() const {
        return maxDepth;
    }

    int getSamplesAccumulated() const {
        return samplesAccumulated;
    }

    void reset() {
        cumulativeRenderTimeMillis = std::chrono::milliseconds(0);
        samplesAccumulated = 0;
        std::fill(cumulativeData.begin(), cumulativeData.end(), Color(0, 0, 0));
    }

    void interrupt() {
        if (!isRendering) {
            return;
        }
        isInterrupted = true;
    }

private:
    std::vector<Color> cumulativeData;
    std::chrono::milliseconds cumulativeRenderTimeMillis = std::chrono::milliseconds(0);
    std::atomic_int samplesAccumulated = 0;
    std::atomic_bool isRendering = false;
    std::atomic_bool isInterrupted = false;

    std::atomic_int imageWidth;
    std::atomic_int imageHeight;
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
        Ray r = camera.getRay(u, v);
        return rayColor(r, scene, maxDepth);
    }
};

#endif//RAYTRACER_RENDERER_H
