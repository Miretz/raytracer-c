#ifndef COMMON_H
#define COMMON_H

#include "vec3.h"
#include "color.h"

typedef struct material {
    int type;
    color albedo;
    double fuzz;
} material;

typedef struct sphere {
    point3 center;
    double radius;
    material mat;
} sphere;

typedef struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    int frontFace;
    material mat;
} hit_record;


#endif