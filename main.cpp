#include "camera.h"
#include "dielectric.h"
#include "gui.h"
#include "hittable.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "ray.h"
#include "renderer.h"
#include "sphere.h"
#include "util.h"
#include <iostream>
#include <thread>

#define M_PI 3.14159265359

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
    const int samplesPerPixel = 1;
    const int maxDepth = 5;
    auto imageWidth = 225;
    auto imageHeight = 150;
    double aspectRatio = static_cast<double>(imageWidth) / imageHeight;

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

    std::shared_ptr<Camera> camera = std::make_shared<Camera>(origin, lookDir, roll, vFov, aspectRatio, aperture, focusDist);
    std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(imageWidth, imageHeight, samplesPerPixel, maxDepth);
    Gui gui(renderer, camera);

    std::thread renderThread([&gui, &renderer, &camera, &world]() {
        while (!gui.isClosing()) {
            auto img = renderer->render(*camera, world);
            gui.setImage(img);
        }
    });

    gui.run();
    renderThread.join();

    std::cerr << "\nDone.\n";
    return 0;
}
