#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "color.h"

typedef struct ray {
    point3 origin;
    vec3 direction;
} ray;

point3 Ray_At(ray* r, double t){
    vec3 d = Vec3_FMul(&r->direction, t);
    Vec3_AddAssign(&d, &r->origin);
    return d;
}

#endif