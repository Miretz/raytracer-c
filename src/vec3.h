#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#include "rtweekend.h"

typedef struct vec3 {
    double e[3];
} vec3;

static inline vec3 Vec3_Neg(const vec3 *v) {
    return (vec3){{-v->e[0], -v->e[1], -v->e[2]}};
}

static inline void Vec3_AddAssign(vec3 *a, const vec3 *b) {
    a->e[0] += b->e[0];
    a->e[1] += b->e[1];
    a->e[2] += b->e[2];
}

static inline void Vec3_SubAssign(vec3 *a, const vec3 *b) {
    a->e[0] -= b->e[0];
    a->e[1] -= b->e[1];
    a->e[2] -= b->e[2];
}

static inline void Vec3_MulAssign(vec3 *a, const vec3 *b) {
    a->e[0] *= b->e[0];
    a->e[1] *= b->e[1];
    a->e[2] *= b->e[2];
}

static inline void Vec3_FMulAssign(vec3 *a, const double t) {
    a->e[0] *= t;
    a->e[1] *= t;
    a->e[2] *= t;
}

static inline void Vec3_FDivAssign(vec3 *a, const double t) {
    Vec3_FMulAssign(a, 1.0 / t);
}

static inline bool Vec3_NearZero(const vec3 *a) {
    static const double s = 1e-8;
    return fabs(a->e[0]) < s && fabs(a->e[1]) < s && fabs(a->e[2]) < s;
}

// Utility functions

static inline vec3 Vec3_Add(const vec3 *a, const vec3 *b) {
    vec3 res = *a;
    Vec3_AddAssign(&res, b);
    return res;
}

static inline vec3 Vec3_Sub(const vec3 *a, const vec3 *b) {
    vec3 res = *a;
    Vec3_SubAssign(&res, b);
    return res;
}

static inline vec3 Vec3_Mul(const vec3 *a, const vec3 *b) {
    vec3 res = *a;
    Vec3_MulAssign(&res, b);
    return res;
}

static inline vec3 Vec3_FMul(const vec3 *a, const double t) {
    vec3 res = *a;
    Vec3_FMulAssign(&res, t);
    return res;
}

static inline vec3 Vec3_Add5(vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 v5) {
    return (vec3){{v1.e[0] + v2.e[0] + v3.e[0] + v4.e[0] + v5.e[0],
                   v1.e[1] + v2.e[1] + v3.e[1] + v4.e[1] + v5.e[1],
                   v1.e[2] + v2.e[2] + v3.e[2] + v4.e[2] + v5.e[2]}};
}

static inline vec3 Vec3_FDiv(const vec3 *a, const double t) {
    return Vec3_FMul(a, 1.0 / t);
}

static inline double Vec3_Dot(const vec3 *u, const vec3 *v) {
    return u->e[0] * v->e[0] + u->e[1] * v->e[1] + u->e[2] * v->e[2];
}

static inline vec3 Vec3_Cross(const vec3 *u, const vec3 *v) {
    return (vec3){{u->e[1] * v->e[2] - u->e[2] * v->e[1],
                   u->e[2] * v->e[0] - u->e[0] * v->e[2],
                   u->e[0] * v->e[1] - u->e[1] * v->e[0]}};
}

static inline double Vec3_LengthSquared(const vec3 *a) {
    double x = a->e[0];
    double y = a->e[1];
    double z = a->e[2];
    return x * x + y * y + z * z;
}

static inline double Vec3_Length(const vec3 *a) {
    return sqrt(Vec3_LengthSquared(a));
}

static inline vec3 Vec3_UnitVector(const vec3 *a) {
    return Vec3_FDiv(a, Vec3_Length(a));
}

static inline vec3 Vec3_Random() {
    return (vec3){{RandomDouble(), RandomDouble(), RandomDouble()}};
}

static inline vec3 Vec3_RandomBetween(const double min, const double max) {
    return (vec3){{RandomBetween(min, max), RandomBetween(min, max),
                   RandomBetween(min, max)}};
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

static inline vec3 Vec3_RandomInUnitDisk(double radius) {
    while (1) {
        const vec3 p = {{RandomBetween(-radius, radius),
                         RandomBetween(-radius, radius), 0.0}};
        if (Vec3_LengthSquared(&p) < radius) {
            return p;
        }
    }
}

typedef vec3 point3;
typedef vec3 color;

#endif