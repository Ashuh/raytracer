#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "ray.h"
#include <optional>

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t;
    bool isFrontFace;

    static HitRecord build(const Ray &r, const Point3 &p, const Vec3 &outwardNormal, double t) {
        bool isFrontFace = dot(r.direction(), outwardNormal) < 0;
        Vec3 normal = isFrontFace ? outwardNormal : -outwardNormal;
        return {p, normal, t, isFrontFace};
    }

    HitRecord(const Point3 &p, const Vec3 &normal, double t, bool isFrontFace) : p(p), normal(normal), t(t), isFrontFace(isFrontFace) {}
};

class Hittable {
public:
    [[nodiscard]] virtual std::optional<HitRecord> hit(const Ray &r, double tMin, double tMax) const = 0;
};

#endif//RAYTRACER_HITTABLE_H
