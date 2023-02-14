#ifndef RAYTRACER_DIELECTRIC_H
#define RAYTRACER_DIELECTRIC_H

#include "hit_record.h"
#include "material.h"

class Dielectric : public Material {
public:
    explicit Dielectric(double ir) : Material(Color(1.0, 1.0, 1.0)), ir(ir) {}

    [[nodiscard]] std::optional<Ray> scatter(const Ray &r, const HitRecord &rec) const override {
        double refraction_ratio = rec.isFrontFace ? (1.0 / ir) : ir;

        if (auto refracted = refract(r.direction(), rec.normal, refraction_ratio)) {
            return Ray(rec.p, *refracted);
        }

        auto reflected = reflect(r.direction(), rec.normal);
        return Ray(rec.p, reflected);
    }

private:
    double ir;// Index of Refraction

    static std::optional<Vec3> refract(const Vec3 &v, const Vec3 &n, double refractionRatio) {
        Vec3 uv = unitVector(v);
        auto cosTheta = std::min(dot(-uv, n), 1.0);
        double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

        bool cannotRefract = refractionRatio * sinTheta > 1.0;
        if (cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble()) {
            return {};
        }

        Vec3 rOutPerp = refractionRatio * (uv + cosTheta * n);
        Vec3 rOutParallel = -sqrt(std::abs(1.0 - rOutPerp.lengthSquared())) * n;
        return rOutPerp + rOutParallel;
    }

    static double reflectance(double cosine, double refractionRatio) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refractionRatio) / (1 + refractionRatio);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif//RAYTRACER_DIELECTRIC_H
