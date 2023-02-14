#ifndef RAYTRACER_LAMBERTIAN_H
#define RAYTRACER_LAMBERTIAN_H

#include "hit_record.h"
#include "material.h"

class Lambertian: public Material {
public:
    explicit Lambertian(const Color &albedo) : Material(albedo) {}

    [[nodiscard]] std::optional<Ray> scatter(const Ray &r, const HitRecord &rec) const override {
        auto scatterDirection = rec.normal + randomUnitVector();
        // Catch degenerate scatter direction
        if (scatterDirection.isNearZero()) {
            scatterDirection = rec.normal;
        }
        auto scattered = Ray(rec.p, scatterDirection);
        return scattered;
    }
};


#endif//RAYTRACER_LAMBERTIAN_H
