#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

#include "rtweekend.h"

typedef struct vec3 {
    double x;
    double y;
    double z;
} vec3;

vec3 Vec3_Neg(vec3 *v) {
    vec3 temp = {-v->x, -v->y, -v->z};
    return temp;
}

void Vec3_AddAssign(vec3 *a, vec3 *b) {
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
}

void Vec3_SubAssign(vec3 *a, vec3 *b) {
    a->x -= b->x;
    a->y -= b->y;
    a->z -= b->z;
}

void Vec3_MulAssign(vec3 *a, double t) {
    a->x *= t;
    a->y *= t;
    a->z *= t;
}

void Vec3_DivAssign(vec3 *a, double t) { Vec3_MulAssign(a, 1.0 / t); }

int Vec3_NearZero(vec3 *a) {
    static const double s = 1e-8;
    return fabs(a->x) < s && fabs(a->y) < s && fabs(a->z) < s;
}

// Utility functions

vec3 Vec3_Sub(vec3 *a, vec3 *b) {
    vec3 result;
    result.x = a->x - b->x;
    result.y = a->y - b->y;
    result.z = a->z - b->z;
    return result;
}

vec3 Vec3_Add(vec3 *a, vec3 *b) {
    vec3 result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    result.z = a->z + b->z;
    return result;
}

vec3 Vec3_AddMultiple(int n, ...) {
    vec3 res = {0, 0, 0};
    va_list ptr;
    va_start(ptr, n);
    for (int i = 0; i < n; i++) {
        vec3 v = va_arg(ptr, vec3);
        res.x += v.x;
        res.y += v.y;
        res.z += v.z;
    }
    va_end(ptr);
    return res;
}

vec3 Vec3_SubMultiple(vec3 start, int n, ...) {
    va_list ptr;
    va_start(ptr, n);
    for (int i = 0; i < n; i++) {
        vec3 v = va_arg(ptr, vec3);
        start.x -= v.x;
        start.y -= v.y;
        start.z -= v.z;
    }
    va_end(ptr);
    return start;
}

vec3 Vec3_Mul(vec3 *a, vec3 *b) {
    vec3 res = {a->x * b->x, a->y * b->y, a->z * b->z};
    return res;
}

vec3 Vec3_FMul(vec3 *a, double t) {
    vec3 res = {a->x * t, a->y * t, a->z * t};
    return res;
}

vec3 Vec3_FDiv(vec3 *a, double t) { return Vec3_FMul(a, 1.0 / t); }

double Vec3_Dot(vec3 *a, vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

vec3 Vec3_Cross(vec3 *u, vec3 *v) {
    vec3 result = {
        u->y * v->z - u->z * v->y,
        u->z * v->x - u->x * v->z,
        u->x * v->y - u->y * v->x,
    };
    return result;
}

double Vec3_LengthSquared(vec3 *a) {
    return a->x * a->x + a->y * a->y + a->z * a->z;
}

double Vec3_Length(vec3 *a) { return sqrt(Vec3_LengthSquared(a)); }

vec3 Vec3_UnitVector(vec3 *a) { return Vec3_FDiv(a, Vec3_Length(a)); }

vec3 Vec3_Random() {
    vec3 res = {RandomDouble(), RandomDouble(), RandomDouble()};
    return res;
}

vec3 Vec3_RandomBetween(double min, double max) {
    vec3 res = {RandomBetween(min, max), RandomBetween(min, max),
                RandomBetween(min, max)};
    return res;
}

vec3 Vec3_RandomInUnitSphere() {
    while (1) {
        vec3 p = Vec3_RandomBetween(-1.0, 1.0);
        if (Vec3_LengthSquared(&p) < 1.0) {
            return p;
        }
    }
}

vec3 Vec3_RandomUnitVector() {
    vec3 t = Vec3_RandomInUnitSphere();
    return Vec3_UnitVector(&t);
}

vec3 Vec3_RandomInHemisphere(vec3 *normal) {
    vec3 inUnitSphere = Vec3_RandomInUnitSphere();
    if (Vec3_Dot(&inUnitSphere, normal) > 0.0) {
        return inUnitSphere;
    } else {
        return Vec3_Neg(&inUnitSphere);
    }
}

vec3 Vec3_Reflect(vec3 *v, vec3 *n) {
    vec3 t1 = Vec3_FMul(n, Vec3_Dot(v, n) * 2.0);
    return Vec3_Sub(v, &t1);
}

vec3 Vec3_Refract(vec3 *uv, vec3 *n, double etaiOverEtat) {
    vec3 negUv = Vec3_Neg(uv);
    double cosTheta = fmin(Vec3_Dot(&negUv, n), 1.0);
    vec3 t = Vec3_FMul(n, cosTheta);
    vec3 tmp = Vec3_Add(uv, &t);
    vec3 rOutPerp = Vec3_FMul(&tmp, etaiOverEtat);
    double parallelMul = -sqrt(fabs(1.0 - Vec3_LengthSquared(&rOutPerp)));
    vec3 rOutParallel = Vec3_FMul(n, parallelMul);
    Vec3_AddAssign(&rOutPerp, &rOutParallel);
    return rOutPerp;
}

vec3 Vec3_RandomInUnitDisk() {
    while (1) {
        vec3 p = {RandomBetween(-1.0, 1.0), RandomBetween(-1.0, 1.0), 0.0};
        if (Vec3_LengthSquared(&p) < 1.0) {
            return p;
        }
    }
}

vec3 Vec3(double x, double y, double z) {
    vec3 res = {x, y, z};
    return res;
}

typedef vec3 point3;

point3 Point3(double x, double y, double z) {
    point3 res = {x, y, z};
    return res;
}

#endif