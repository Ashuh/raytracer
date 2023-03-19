cmake_minimum_required(VERSION 3.8)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracer main.cpp vec3.h color.h ray.h hittable.h sphere.h hittable_list.h util.h camera.h material.h lambertian.h metal.h dielectric.h renderer.h gui.h image.h render_manager.h gui_listener.h)

find_package(glad CONFIG REQUIRED)
target_link_libraries(raytracer PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(raytracer PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(raytracer PRIVATE imgui::imgui)