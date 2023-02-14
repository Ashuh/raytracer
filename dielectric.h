#ifndef RAYTRACER_DIELECTRIC_H
#define RAYTRACER_DIELECTRIC_H

#include "hit_record.h"
#include "material.h"

class Dielectric : public Material {
public:
    explicit Dielectric(double ir) : Material(Color(1.0, 1.0, 1.0)), ir(ir) {}

    [[nodiscard]] std::optional<Ray> scatter(const Ray &r, const HitRecord &rec) const override {
        double refraction_ratio = rec.isFrontFace ? (1.0 / ir) : ir;
        Vec3 refracted = refract(r.direction(), rec.normal, refraction_ratio);
        return Ray(rec.p, refracted);
    }

public:
    double ir;// Index of Refraction
};

#endif//RAYTRACER_DIELECTRIC_H
