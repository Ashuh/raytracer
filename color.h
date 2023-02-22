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

    // gamma correction
    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);

    out << static_cast<int>(256 * std::clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(b, 0.0, 0.999)) << '\n';
}

int toInt(const Color& color) {
    auto r = color.x();
    auto g = color.y();
    auto b = color.z();

    // gamma correction
    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);

    auto rr = static_cast<int> (256 * std::clamp(r, 0.0, 0.999));
    auto gg = static_cast<int>(256 * std::clamp(g, 0.0, 0.999));
    auto bb = static_cast<int>(256 * std::clamp(b, 0.0, 0.999));
    auto a = 255;
    return (a << 24) + (bb << 16) + (gg << 8) + rr;
}

#endif//RAYTRACER_COLOR_H
