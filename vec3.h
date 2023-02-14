#ifndef RAYTRACER_VEC3_H
#define RAYTRACER_VEC3_H

#include "util.h"
#include <array>
#include <cmath>
#include <ostream>

class Vec3 {
public:
    inline static Vec3 random() {
        return {randomDouble(), randomDouble(), randomDouble()};
    }

    inline static Vec3 random(double min, double max) {
        return {randomDouble(min, max), randomDouble(min, max), randomDouble(min, max)};
    }

    Vec3() : e{0, 0, 0} {}

    Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    [[nodiscard]] double x() const { return e[0]; }

    [[nodiscard]] double y() const { return e[1]; }

    [[nodiscard]] double z() const { return e[2]; }

    Vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }

    double operator[](int i) const { return e[i]; }

    double &operator[](int i) { return e[i]; }

    Vec3 &operator+=(const Vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vec3 &operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3 &operator/=(const double t) {
        return *this *= 1 / t;
    }


    [[nodiscard]] double length() const {
        return sqrt(lengthSquared());
    }

    [[nodiscard]] double lengthSquared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    /**
     *
     * @return true if the vector is close to zero in all dimensions.
     */
    [[nodiscard]] bool isNearZero() const {
        const auto s = 1e-8;
        return (std::abs(e[0]) < s) && (std::abs(e[1]) < s) && (std::abs(e[2]) < s);
    }

private:
    std::array<double, 3> e;
};

// Type aliases for Vec3
using Point3 = Vec3;// 3D point
using Color = Vec3; // RGB color

// Vec3 Utility Functions

inline std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
    return {u[0] + v[0], u[1] + v[1], u[2] + v[2]};
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
    return {u[0] - v[0], u[1] - v[1], u[2] - v[2]};
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return {u[0] * v[0], u[1] * v[1], u[2] * v[2]};
}

inline Vec3 operator*(double t, const Vec3 &v) {
    return {t * v[0], t * v[1], t * v[2]};
}

inline Vec3 operator*(const Vec3 &v, double t) {
    return t * v;
}

inline Vec3 operator/(const Vec3 &v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vec3 &u, const Vec3 &v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return {u[1] * v[2] - u[2] * v[1],
            u[2] * v[0] - u[0] * v[2],
            u[0] * v[1] - u[1] * v[0]};
}

inline Vec3 unitVector(const Vec3 &v) {
    return v / v.length();
}

Vec3 randomInUnitSphere() {
    while (true) {
        auto p = Vec3::random(-1, 1);
        if (p.lengthSquared() >= 1) {
            continue;
        }
        return p;
    }
}

inline Vec3 randomUnitVector() {
    return unitVector(randomInUnitSphere());
}

inline Vec3 reflect(const Vec3 &v, const Vec3 &n) {
    return v - 2 * dot(v, n) * n;
}

Vec3 refract(const Vec3 &v, const Vec3 &n, double refractionRatio) {
    Vec3 uv = unitVector(v);
    auto cosTheta = std::min(dot(-uv, n), 1.0);
    Vec3 rOutPerp = refractionRatio * (uv + cosTheta * n);
    Vec3 rOutParallel = -std::sqrt(fabs(1.0 - rOutPerp.lengthSquared())) * n;
    return rOutPerp + rOutParallel;
}

#endif//RAYTRACER_VEC3_H
