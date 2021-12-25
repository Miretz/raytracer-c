#ifndef HITTABLE_H
#define HITTABLE_H

#include "material.h"
#include "ray.h"
#include "vec3.h"


static inline void
Hittable_SetFaceNormal(const ray *r, const vec3 *outwardNormal, hit_record *h) {
    if (Vec3_Dot(&r->direction, outwardNormal) < 0.0) {
        h->normal = *outwardNormal;
        h->frontFace = true;
    } else {
        h->normal = Vec3_Neg(outwardNormal);
        h->frontFace = false;
    }
}

#endif