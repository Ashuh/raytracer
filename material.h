#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "hit_record.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include <optional>

struct HitRecord;

class Material {
public:
    explicit Material(const Color &albedo) : albedo(albedo) {}

    [[nodiscard]] virtual std::optional<Ray> scatter(const Ray &r, const HitRecord &rec) const = 0;

    [[nodiscard]] const Color &getAlbedo() const {
        return albedo;
    }

protected:
    Color albedo;

    static inline Vec3 reflect(const Vec3 &v, const Vec3 &n) {
        return v - 2 * dot(v, n) * n;
    }
};

#endif//RAYTRACER_MATERIAL_H
