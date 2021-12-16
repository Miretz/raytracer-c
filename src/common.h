#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

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
    double t;
    vec3 normal;
    bool frontFace;
    material* matPtr;
} hit_record;


#endif