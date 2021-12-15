#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"
#include "material.h"

static inline void Hittable_SetFaceNormal(const ray *r, const vec3 *outwardNormal, hit_record *h) {
    h->frontFace = Vec3_Dot(&r->direction, outwardNormal) < 0.0;
    if (h->frontFace) {
        h->normal = *outwardNormal;
    } else {
        h->normal = Vec3_Neg(outwardNormal);
    }
}

#endif