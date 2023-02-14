#ifndef RAYTRACER_HITTABLE_LIST_H
#define RAYTRACER_HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
public:
    HittableList() = default;

    explicit HittableList(const shared_ptr<Hittable> &object) { add(object); }

    void clear() { objects.clear(); }

    void add(const shared_ptr<Hittable> &object) { objects.push_back(object); }

    [[nodiscard]] std::optional<HitRecord> hit(const Ray &r, double tMin, double tMax) const override;

public:
    std::vector<shared_ptr<Hittable>> objects;
};

std::optional<HitRecord> HittableList::hit(const Ray &r, double tMin, double tMax) const {
    std::optional<HitRecord> result;
    auto nearestHitDist = tMax;

    for (const auto &object: objects) {
        if (auto rec = object->hit(r, tMin, nearestHitDist)) {
            nearestHitDist = rec->t;
            result = rec;
        }
    }

    return result;
}

#endif//RAYTRACER_HITTABLE_LIST_H
