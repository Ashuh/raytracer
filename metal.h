#ifndef RAYTRACER_METAL_H
#define RAYTRACER_METAL_H

#include "material.h"

class Metal : public Material {
public:
    explicit Metal(const Color &albedo) : Material(albedo) {}

    [[nodiscard]] std::optional<Ray> scatter(const Ray &r, const HitRecord &rec) const override {
        Vec3 reflected = reflect(unitVector(r.direction()), rec.normal);
        auto scattered = Ray(rec.p, reflected);

        if (dot(scattered.direction(), rec.normal) > 0) {
            return scattered;
        }

        return {};
    }
};

#endif//RAYTRACER_METAL_H
