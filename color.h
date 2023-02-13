#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H

#include "vec3.h"
#include <ostream>

void writeColor(std::ostream &out, Color color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * color.x()) << ' '
        << static_cast<int>(255.999 * color.y()) << ' '
        << static_cast<int>(255.999 * color.z()) << '\n';
}

#endif//RAYTRACER_COLOR_H
