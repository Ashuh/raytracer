#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "ray.h"
#include "vec3.h"

class Camera {
public:
    Camera(const Point3 &origin, double viewportWidth, double viewportHeight, double focalLength) : origin(origin) {
        horizontal = Vec3(viewportWidth, 0.0, 0.0);
        vertical = Vec3(0.0, viewportHeight, 0.0);
        lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);
    }

    [[nodiscard]] Ray getRay(double u, double v) const {
        return {origin, lowerLeftCorner + u * horizontal + v * vertical - origin};
    }

private:
    Point3 origin;
    Point3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
};

#endif//RAYTRACER_CAMERA_H
