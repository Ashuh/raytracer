#ifndef RAYTRACER_UTIL_H
#define RAYTRACER_UTIL_H

#include <algorithm>
#include <random>

/**
 *
 * @return A random real in [0,1).
 */
inline double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

/**
 *
 * @return A random real in [min,max).
 */
inline double randomDouble(double min, double max) {
    return min + (max - min) * randomDouble();
}

#endif//RAYTRACER_UTIL_H
