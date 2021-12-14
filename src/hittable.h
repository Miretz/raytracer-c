#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"
#include "material.h"

void Hittable_SetFaceNormal(hit_record *h, ray *r, vec3 *outwardNormal) {
    h->frontFace = Vec3_Dot(&r->direction, outwardNormal) < 0.0;
    if (h->frontFace) {
        h->normal = *outwardNormal;
    } else {
        h->normal = Vec3_Neg(outwardNormal);
    }
}

#endif