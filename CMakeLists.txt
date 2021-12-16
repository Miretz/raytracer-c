cmake_minimum_required(VERSION 3.0.0)
project(raytracer-c C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -Wall -Wextra -O3 -ffast-math -msse -mavx2 -ftree-vectorize")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

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
target_link_libraries(raytracer-c PRIVATE Threads::Threads)