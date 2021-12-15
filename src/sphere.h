#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "hittable.h"
#include "material.h"
#include "vec3.h"

static inline unsigned int Sphere_Hit(sphere *s, const ray *r, const double tMin,
               const double tMax, hit_record *rec) {
    const vec3 oc = Vec3_Sub(&r->origin, &s->center);
    const double halfB = Vec3_Dot(&oc, &r->direction);
    const double c = Vec3_LengthSquared(&oc) - s->radius * s->radius;
    const double a = Vec3_LengthSquared(&r->direction);
    const double discriminant = halfB * halfB - a * c;
    if (discriminant < 0.0) {
        return 0;
    }
    const double sqrtd = sqrt(discriminant);

    double root = (-halfB - sqrtd) / a;
    if (root < tMin || tMax < root) {
        root = (-halfB + sqrtd) / a;
        if (root < tMin || tMax < root) {
            return 0;
        }
    }

    rec->t = root;
    rec->p = Ray_At(r, rec->t);

    vec3 outwardNormal = Vec3_Sub(&rec->p, &s->center);
    Vec3_FDivAssign(&outwardNormal, s->radius);
    Hittable_SetFaceNormal(r, &outwardNormal, rec);
    rec->matPtr = &s->mat;
    return 1;
}

static inline sphere NewSphere(point3 center, double radius, material mat) {
    sphere s;
    s.center = center;
    s.radius = radius;
    s.mat = mat;
    return s;
}

#endif