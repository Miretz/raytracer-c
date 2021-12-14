cmake_minimum_required(VERSION 3.0.0)
project(raytracer-c C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2 -g")

set(SOURCES
    src/rtweekend.h
    src/common.h
    src/vec3.h
    src/camera.h
    src/color.h
    src/hittable.h
    src/hittable_list.h
    src/material.h
    src/ray.h
    src/sphere.h
    src/main.c    
)

add_executable(raytracer-c ${SOURCES})