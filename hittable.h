#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "material.h"
#include "ray.h"
#include <memory>
#include <optional>

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t;
    bool isFrontFace;
    std::shared_ptr<Material> material;

    static HitRecord build(const Ray &r, const Point3 &p, const Vec3 &outwardNormal, double t, const std::shared_ptr<Material> &m) {
        bool isFrontFace = dot(r.direction(), outwardNormal) < 0;
        Vec3 normal = isFrontFace ? outwardNormal : -outwardNormal;
        return {p, normal, t, isFrontFace, m};
    }

    HitRecord(const Point3 &p, const Vec3 &normal, double t, bool isFrontFace, const std::shared_ptr<Material> &m) : p(p),
                                                                                                                     normal(normal),
                                                                                                                     t(t),
                                                                                                                     isFrontFace(isFrontFace),
                                                                                                                     material(m) {}
};

class Hittable {
public:
    [[nodiscard]] virtual std::optional<HitRecord> hit(const Ray &r, double tMin, double tMax) const = 0;
};

#endif//RAYTRACER_HITTABLE_H
