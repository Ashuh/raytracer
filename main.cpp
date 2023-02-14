#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"
#include "sphere.h"
#include "util.h"
#include <iostream>

Color rayColor(const Ray &r, const Hittable &world, int depth) {
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        return {0, 0, 0};
    }

    if (auto rec = world.hit(r, 0.001, std::numeric_limits<double>::infinity())) {
        Point3 target = rec->p + rec->normal + randomUnitVector();
        Ray reflectedRay = Ray(rec->p, target - rec->p);
        return 0.5 * rayColor(reflectedRay, world, depth - 1);
    }
    Vec3 unit_direction = unitVector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samplesPerPixel = 100;
    const int maxDepth = 50;

    // World
    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera
    auto origin = Point3(0, 0, 0);
    auto viewportHeight = 2.0;
    auto viewportWidth = aspect_ratio * viewportHeight;
    auto focalLength = 1.0;
    Camera cam(origin, viewportWidth, viewportHeight, focalLength);

    // Render
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {

            Color pixelColor(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                auto u = (i + randomDouble()) / (image_width - 1);
                auto v = (j + randomDouble()) / (image_height - 1);
                Ray r = cam.getRay(u, v);
                pixelColor += rayColor(r, world, maxDepth);
            }

            pixelColor /= samplesPerPixel;
            writeColor(std::cout, pixelColor);
        }
    }

    std::cerr << "\nDone.\n";
    return 0;
}
