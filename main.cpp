#include "camera.h"
#include "color.h"
#include "dielectric.h"
#include "hittable.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "ray.h"
#include "renderer.h"
#include "sphere.h"
#include "util.h"
#include <iostream>

HittableList randomScene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto chooseMat = randomDouble();
            Point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                } else if (chooseMat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}


int main() {
    // Image
    const auto aspectRatio = 3.0 / 2.0;
    const int imageWidth = 600;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 1;
    const int maxDepth = 10;

    // World
    auto world = randomScene();

    // Camera
    auto origin = Point3(13, 2, 3);
    auto lookAt = Point3(0, 0, 0);
    auto lookDir = lookAt - origin;
    auto verticalDir = Point3(0, 1, 0);
    auto roll = 0 * M_PI / 180;
    auto vFov = 20 * M_PI / 180;
    auto aperture = 0.1;
    auto focusDist = 10.0;
    Camera cam(origin, lookDir, roll, vFov, aspectRatio, aperture, focusDist);

    Renderer renderer(imageWidth, imageHeight, samplesPerPixel, maxDepth);
    renderer.render(cam, world);

    // Render
    std::cout << "P3\n"
              << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int i = 0; i < imageHeight * imageWidth; ++i) {
        writeColor(std::cout, renderer.image[i]);
    }

    std::cerr << "\nDone.\n";
    return 0;
}
