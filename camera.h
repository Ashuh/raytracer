#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "ray.h"
#include "vec3.h"
#include <atomic>

class Camera {
public:
    /**
     *
     * @param vFov vertical field of view in radians.
     * @param focusDist distance between focus plane and projection point
     */
    Camera(const Point3 &origin, const Vec3 &lookDir, double roll, double vFov,
           double aspectRatio, double aperture, double focusDist) : origin(origin), lensRadius(aperture / 2) {
        auto viewportHeight = 2 * focusDist * tan(vFov / 2);
        auto viewportWidth = aspectRatio * viewportHeight;

        auto vup = Vec3(-sin(roll), cos(roll), 0);
        auto unitLookDir = unitVector(lookDir);
        unitHorizontal = unitVector(cross(vup, -unitLookDir));
        unitVertical = cross(-unitLookDir, unitHorizontal);

        horizontal = viewportWidth * unitHorizontal;
        vertical = viewportHeight * unitVertical;
        lowerLeftCorner = origin - horizontal / 2 - vertical / 2 + unitLookDir * focusDist;
    }

    [[nodiscard]] Ray getRay(double s, double t) const {
        Vec3 rd = lensRadius * randomInUnitDisk();
        Vec3 offset = unitHorizontal * rd.x() + unitVertical * rd.y();
        return {origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset};
    }

    [[nodiscard]] const Point3 &getOrigin() const {
        return origin;
    }

    void setOrigin(const Point3 &point) {
        origin = point;
    }

    [[nodiscard]] double getLensRadius() const {
        return lensRadius;
    }

    void setLensRadius(double radius) {
        lensRadius = radius;
    }

private:
    Point3 origin;
    Point3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 unitHorizontal;
    Vec3 unitVertical;
    std::atomic<double> lensRadius;
};

#endif//RAYTRACER_CAMERA_H
