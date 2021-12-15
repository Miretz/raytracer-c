#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "color.h"

typedef struct ray {
    point3 origin;
    vec3 direction;
} ray;

static inline point3 Ray_At(const ray* r, const double t){
    vec3 d = r->direction;
    Vec3_FMulAssign(&d, t);
    Vec3_AddAssign(&d, &r->origin);
    return d;
}

#endif