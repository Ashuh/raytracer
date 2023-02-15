#include "camera.h"
#include "color.h"
#include "dielectric.h"
#include "hittable.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metal.h"
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
        if (auto scattered = rec->material->scatter(r, *rec)) {
            return rec->material->getAlbedo() * rayColor(*scattered, world, depth - 1);
        }
        return {0, 0, 0};
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

    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera
    auto origin = Point3(-2, 2, 1);
    auto lookDir = Point3(2, -2, -2);
    auto verticalDir = Point3(0, 1, 0);
    auto focalLength = 1.0;
    Camera cam(origin, lookDir, 0 * M_PI / 180, 20 * M_PI / 180, aspect_ratio, focalLength);

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
