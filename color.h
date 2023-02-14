#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H

#include "vec3.h"
#include <algorithm>
#include <ostream>

void writeColor(std::ostream &out, Color color) {
    // Write the translated [0,255] value of each color component.
    auto r = color.x();
    auto g = color.y();
    auto b = color.z();

    out << static_cast<int>(256 * std::clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(b, 0.0, 0.999)) << '\n';
}

#endif//RAYTRACER_COLOR_H
