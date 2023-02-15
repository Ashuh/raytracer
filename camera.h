#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "ray.h"
#include "vec3.h"

class Camera {
public:
    /**
     *
     * @param vFov vertical field of view in radians.
     * @param focalLength distance between projection plane and projection point
     */
    Camera(const Point3 &origin, const Vec3 &lookDir, double roll, double vFov, double aspectRatio, double focalLength) : origin(origin) {
        auto viewportHeight = 2 * focalLength * tan(vFov / 2);
        auto viewportWidth = aspectRatio * viewportHeight;

        auto uv = Vec3(-sin(roll), cos(roll), 0);
        auto unitLookDir = unitVector(lookDir);
        auto unitHorizontal = unitVector(cross(uv, -unitLookDir));
        auto unitVertical = cross(-unitLookDir, unitHorizontal);

        horizontal = viewportWidth * unitHorizontal;
        vertical = viewportHeight * unitVertical;
        lowerLeftCorner = origin - horizontal / 2 - vertical / 2 + unitLookDir * focalLength;
    }

    [[nodiscard]] Ray getRay(double u, double v) const {
        return {origin, lowerLeftCorner + u * horizontal + v * vertical - origin};
    }

private:
    Point3 origin;
    Point3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
};

#endif//RAYTRACER_CAMERA_H
