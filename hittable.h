#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "hit_record.h"
#include "ray.h"
#include <memory>
#include <optional>

struct HitRecord;

class Hittable {
public:
    [[nodiscard]] virtual std::optional<HitRecord> hit(const Ray &r, double tMin, double tMax) const = 0;
};

#endif//RAYTRACER_HITTABLE_H
