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

static inline vec3 Vec3_Neg(const vec3 *v) {
    return (vec3){-v->x, -v->y, -v->z};
}

static inline void Vec3_AddAssign(vec3 *a, const vec3 *b) {
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
}

static inline void Vec3_SubAssign(vec3 *a, const vec3 *b) {
    a->x -= b->x;
    a->y -= b->y;
    a->z -= b->z;
}

static inline void Vec3_MulAssign(vec3 *a, const vec3 *b) {
    a->x *= b->x;
    a->y *= b->y;
    a->z *= b->z;
}

static inline void Vec3_FMulAssign(vec3 *a, const double t) {
    a->x *= t;
    a->y *= t;
    a->z *= t;
}

static inline void Vec3_FDivAssign(vec3 *a, const double t) {
    Vec3_FMulAssign(a, 1.0 / t);
}

static inline unsigned int Vec3_NearZero(const vec3 *a) {
    static const double s = 1e-8;
    return fabs(a->x) < s && fabs(a->y) < s && fabs(a->z) < s;
}

// Utility functions

static inline vec3 Vec3_Sub(const vec3 *a, const vec3 *b) {
    vec3 res = *a;
    res.x -= b->x;
    res.y -= b->y;
    res.z -= b->z;
    return res;
}

static inline vec3 Vec3_Add(const vec3 *a, const vec3 *b) {
    vec3 res = *a;
    res.x += b->x;
    res.y += b->y;
    res.z += b->z;
    return res;
}

static inline vec3 Vec3_Mul(const vec3 *a, const vec3 *b) {
    vec3 res = *a;
    res.x *= b->x;
    res.y *= b->y;
    res.z *= b->z;
    return res;
}

static inline vec3 Vec3_FMul(const vec3 *a, const double t) {
    vec3 res = *a;
    res.x *= t;
    res.y *= t;
    res.z *= t;
    return res;
}

static inline vec3 Vec3_AddMultiple(const int n, ...) {
    vec3 res = {0, 0, 0};
    va_list ptr = NULL;
    va_start(ptr, n);
    for (int i = n; i; i--) {
        vec3 v = va_arg(ptr, vec3);
        res.x += v.x;
        res.y += v.y;
        res.z += v.z;
    }
    va_end(ptr);
    return res;
}

static inline vec3 Vec3_SubMultiple(vec3 start, const int n, ...) {
    va_list ptr = NULL;
    va_start(ptr, n);
    for (int i = n; i; i--) {
        vec3 v = va_arg(ptr, vec3);
        start.x -= v.x;
        start.y -= v.y;
        start.z -= v.z;
    }
    va_end(ptr);
    return start;
}

static inline vec3 Vec3_FDiv(const vec3 *a, const double t) {
    return Vec3_FMul(a, 1.0 / t);
}

static inline double Vec3_Dot(const vec3 *a, const vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

static inline vec3 Vec3_Cross(const vec3 *u, const vec3 *v) {
    return (vec3){
        u->y * v->z - u->z * v->y,
        u->z * v->x - u->x * v->z,
        u->x * v->y - u->y * v->x,
    };
}

static inline double Vec3_LengthSquared(const vec3 *a) {
    double x = a->x;
    double y = a->y;
    double z = a->z;
    return x * x + y * y + z * z;
}

static inline double Vec3_Length(const vec3 *a) {
    return sqrt(Vec3_LengthSquared(a));
}

static inline vec3 Vec3_UnitVector(const vec3 *a) {
    return Vec3_FDiv(a, Vec3_Length(a));
}

static inline vec3 Vec3_Random() {
    return (vec3){RandomDouble(), RandomDouble(), RandomDouble()};
}

static inline vec3 Vec3_RandomBetween(const double min, const double max) {
    return (vec3){RandomBetween(min, max), RandomBetween(min, max),
                  RandomBetween(min, max)};
}

static inline vec3 Vec3_RandomInUnitSphere() {
    while (1) {
        vec3 p = Vec3_RandomBetween(-1.0, 1.0);
        if (Vec3_LengthSquared(&p) < 1.0) {
            return p;
        }
    }
}

static inline vec3 Vec3_RandomUnitVector() {
    const vec3 tmp = Vec3_RandomInUnitSphere();
    return Vec3_UnitVector(&tmp);
}

static inline vec3 Vec3_RandomInHemisphere(const vec3 *normal) {
    vec3 inUnitSphere = Vec3_RandomInUnitSphere();
    if (Vec3_Dot(&inUnitSphere, normal) > 0.0) {
        return inUnitSphere;
    } else {
        return Vec3_Neg(&inUnitSphere);
    }
}

static inline vec3 Vec3_Reflect(const vec3 *v, const vec3 *n) {
    const vec3 t1 = Vec3_FMul(n, Vec3_Dot(v, n) * 2.0);
    return Vec3_Sub(v, &t1);
}

static inline vec3 Vec3_Refract(const vec3 *uv, const vec3 *n,
                                const double etaiOverEtat) {
    const vec3 negUv = Vec3_Neg(uv);
    const vec3 t = Vec3_FMul(n, fmin(Vec3_Dot(&negUv, n), 1.0));
    const vec3 tmp = Vec3_Add(uv, &t);
    vec3 rOutPerp = Vec3_FMul(&tmp, etaiOverEtat);
    const double parallelMul = -sqrt(fabs(1.0 - Vec3_LengthSquared(&rOutPerp)));
    const vec3 rOutParallel = Vec3_FMul(n, parallelMul);
    Vec3_AddAssign(&rOutPerp, &rOutParallel);
    return rOutPerp;
}

static inline vec3 Vec3_RandomInUnitDisk() {
    while (1) {
        const vec3 p = {RandomBetween(-1.0, 1.0), RandomBetween(-1.0, 1.0),
                        0.0};
        if (Vec3_LengthSquared(&p) < 1.0) {
            return p;
        }
    }
}

typedef vec3 point3;
typedef vec3 color;

#endif