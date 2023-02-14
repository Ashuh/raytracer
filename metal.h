#ifndef RAYTRACER_METAL_H
#define RAYTRACER_METAL_H

#include "hit_record.h"
#include "material.h"
#include <cassert>

class Metal : public Material {
public:
    explicit Metal(const Color &albedo, double f) : Material(albedo), fuzz(f) {}

    [[nodiscard]] std::optional<Ray> scatter(const Ray &r, const HitRecord &rec) const override {
        Vec3 reflected = reflect(unitVector(r.direction()), rec.normal);
        auto scattered = Ray(rec.p, reflected + fuzz * randomInUnitSphere());

        if (dot(scattered.direction(), rec.normal) > 0) {
            return scattered;
        }

        return {};
    }

private:
    double fuzz;
};

#endif//RAYTRACER_METAL_H
