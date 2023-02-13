#ifndef RAYTRACER_VEC3_H
#define RAYTRACER_VEC3_H

#include <array>
#include <cmath>
#include <ostream>

class Vec3 {
public:
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
        return sqrt(length_squared());
    }

    [[nodiscard]] double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
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

inline Vec3 operator/(Vec3 v, double t) {
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

inline Vec3 unitVector(Vec3 v) {
    return v / v.length();
}

#endif//RAYTRACER_VEC3_H
