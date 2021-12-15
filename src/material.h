#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "ray.h"
#include "vec3.h"
#include <stdlib.h>

// Lambertian

static inline unsigned int Lambertian_Scatter(const material *l, const hit_record *rec,
                       color *attenuation, ray *scattered) {
    vec3 randomUnit = Vec3_RandomUnitVector();
    vec3 scatterDirection = Vec3_Add(&rec->normal, &randomUnit);
    if (Vec3_NearZero(&scatterDirection) == 1) {
        scatterDirection = rec->normal;
    }
    scattered->origin = rec->p;
    scattered->direction = scatterDirection;
    *attenuation = l->albedo;
    return 1;
}

// Metal

static inline unsigned int Metal_Scatter(const material *l, const ray *rayIn, const hit_record *rec,
                  color *attenuation, ray *scattered) {

    vec3 unitDir = Vec3_UnitVector(&rayIn->direction);
    vec3 reflected = Vec3_Reflect(&unitDir, &rec->normal);
    vec3 randomInUnit = Vec3_RandomInUnitSphere();
    Vec3_FMulAssign(&randomInUnit, l->fuzz);
    ray scResult = {rec->p, Vec3_Add(&reflected, &randomInUnit)};
    *scattered = scResult;
    *attenuation = l->albedo;
    return Vec3_Dot(&scattered->direction, &rec->normal) > 0;
}

// Dielectric

static inline double Dielectric_Reflectance(const double cosine, const double refIdx) {
    double r0 = (1 - refIdx) / (1 + refIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

static inline unsigned int Dielectric_Scatter(const material *l, const ray *rayIn, const hit_record *rec,
                       color *attenuation, ray *scattered) {
    *attenuation = (color){1.0, 1.0, 1.0};
    double refractionRatio = l->fuzz;
    if (rec->frontFace) {
        refractionRatio = 1.0 / l->fuzz;
    }
    vec3 unitDirection = Vec3_UnitVector(&rayIn->direction);
    vec3 unitDirNeg = Vec3_Neg(&unitDirection);
    double cosTheta = fmin(Vec3_Dot(&unitDirNeg, &rec->normal), 1.0);
    double sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    int cannotRefract = refractionRatio * sinTheta > 1.0;

    if (cannotRefract == 1 ||
        Dielectric_Reflectance(cosTheta, refractionRatio) > RandomDouble()) {
        ray res = {rec->p, Vec3_Reflect(&unitDirection, &rec->normal)};
        *scattered = res;
    } else {
        ray res = {rec->p,
                   Vec3_Refract(&unitDirection, &rec->normal, refractionRatio)};
        *scattered = res;
    }

    return 1;
}

static inline unsigned int Mat_Scatter(const material *l, const ray *rayIn, const hit_record *rec, color *attenuation,
                ray *scattered) {

    if (l->type == 1) {
        return Metal_Scatter(l, rayIn, rec, attenuation, scattered);
    } else if (l->type == 2) {
        return Dielectric_Scatter(l, rayIn, rec, attenuation, scattered);
    } else {
        return Lambertian_Scatter(l, rec, attenuation, scattered);
    }
}

static inline material NewMaterial(int type, color albedo, double fuzz) {
    material l;
    l.type = type;
    l.albedo = albedo;
    l.fuzz = fuzz;
    return l;
}

#endif