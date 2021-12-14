#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "hittable.h"
#include "material.h"
#include "vec3.h"


int Sphere_Hit(sphere *s, ray *r, double tMin, double tMax, hit_record *rec) {
    vec3 oc = Vec3_Sub(&r->origin, &s->center);
    double halfB = Vec3_Dot(&oc, &r->direction);
    double c = Vec3_LengthSquared(&oc) - s->radius * s->radius;
    double a = Vec3_LengthSquared(&r->direction);
    double discriminant = halfB * halfB - a * c;
    if (discriminant < 0.0) {
        return 0;
    }
    double sqrtd = sqrt(discriminant);
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
    Vec3_DivAssign(&outwardNormal, s->radius);
    Hittable_SetFaceNormal(rec, r, &outwardNormal);
    rec->mat = s->mat;
    return 1;
}

sphere NewSphere(point3 center, double radius, material mat) {
    sphere s;
    s.center = center;
    s.radius = radius;
    s.mat = mat;
    return s;
}

#endif