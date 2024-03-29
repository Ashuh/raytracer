#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "hit_record.h"
#include "hittable.h"
#include <optional>
#include <tuple>

class Sphere : public Hittable {
public:
    Sphere() = default;

    Sphere(const Point3 &cen, double r, const std::shared_ptr<Material> &m) : center(cen), radius(r), material(m) {}

    [[nodiscard]] std::optional<HitRecord> hit(const Ray &r, double tMin, double tMax) const override;

private:
    Point3 center;
    double radius;
    std::shared_ptr<Material> material;
};

std::optional<HitRecord> Sphere::hit(const Ray &r, double tMin, double tMax) const {
    Vec3 oc = r.origin() - center;
    auto a = r.direction().lengthSquared();
    auto halfB = dot(oc, r.direction());
    auto c = oc.lengthSquared() - radius * radius;

    auto discriminant = halfB * halfB - a * c;
    if (discriminant < 0) {
        return {};
    }
    auto sqrtd = sqrt(discriminant);

    // Find the nearest t that lies in the acceptable range.
    auto t = (-halfB - sqrtd) / a;
    if (t < tMin || tMax < t) {
        t = (-halfB + sqrtd) / a;
        if (t < tMin || tMax < t)
            return {};
    }

    auto p = r.at(t);
    auto normal = (p - center) / radius;
    return HitRecord::build(r, p, normal, t, material);
}

#endif//RAYTRACER_SPHERE_H
