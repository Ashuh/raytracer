cmake_minimum_required(VERSION 3.24)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracer main.cpp vec3.h color.h ray.h hittable.h sphere.h hittable_list.h util.h camera.h material.h lambertian.h metal.h)
