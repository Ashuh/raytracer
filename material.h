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

    virtual std::optional<Ray> scatter(const Ray &r, const HitRecord &rec) const = 0;

    [[nodiscard]] const Color &getAlbedo() const {
        return albedo;
    }

protected:
    Color albedo;
};

#endif//RAYTRACER_MATERIAL_H
