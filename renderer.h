#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

#include "camera.h"
#include "hittable_list.h"

class Renderer {
public:
    Color *image;

    Renderer(int imageWidth, int imageHeight, int samplesPerPixel, int maxDepth) : imageWidth(imageWidth),
                                                                                   imageHeight(imageHeight),
                                                                                   samplesPerPixel(samplesPerPixel),
                                                                                   maxDepth(maxDepth) {
        image = new Color[imageHeight * imageWidth];
    }

    void render(const Camera &camera, const HittableList &scene) {
        int idx = 0;

        for (int j = imageHeight - 1; j >= 0; --j) {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            auto v = (j + randomDouble()) / (imageHeight - 1);
            for (int i = 0; i < imageWidth; ++i) {
                auto u = (i + randomDouble()) / (imageWidth - 1);
                Color color = pixelColor(scene, camera, u, v);
                image[idx++] = color;
            }
        }
    }

private:
    int imageWidth;
    int imageHeight;
    int samplesPerPixel;
    int maxDepth;

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
