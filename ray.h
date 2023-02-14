#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "vec3.h"

class Ray {
public:
    Ray() = default;

    Ray(const Point3 &origin, const Vec3 &direction)
        : orig(origin), dir(direction) {}

    [[nodiscard]] Point3 origin() const { return orig; }

    [[nodiscard]] Vec3 direction() const { return dir; }

    [[nodiscard]] Point3 at(double t) const {
        return orig + t * dir;
    }

private:
    Point3 orig;
    Vec3 dir;
};

#endif//RAYTRACER_RAY_H
