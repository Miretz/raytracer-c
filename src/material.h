#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdbool.h>
#include <stdlib.h>

#include "common.h"
#include "ray.h"
#include "vec3.h"

// Lambertian

static inline bool Lambertian_Scatter(const material *l, const hit_record *rec,
                                      color *attenuation, ray *scattered) {
    const vec3 randomUnit = Vec3_RandomUnitVector();
    vec3 scatterDirection = Vec3_Add(&rec->normal, &randomUnit);
    if (Vec3_NearZero(&scatterDirection)) {
        scatterDirection = rec->normal;
    }
    *scattered = (ray){rec->p, scatterDirection};
    *attenuation = l->albedo;
    return true;
}

// Metal

static inline bool Metal_Scatter(const material *l, const ray *rayIn,
                                 const hit_record *rec, color *attenuation,
                                 ray *scattered) {

    const vec3 unitDir = Vec3_UnitVector(&rayIn->direction);
    const vec3 reflected = Vec3_Reflect(&unitDir, &rec->normal);
    vec3 randomInUnit = Vec3_RandomInUnitSphere();
    Vec3_FMulAssign(&randomInUnit, l->fuzz);
    const vec3 scatterDirection = Vec3_Add(&reflected, &randomInUnit);
    *scattered = (ray){rec->p, scatterDirection};
    *attenuation = l->albedo;
    return Vec3_Dot(&scatterDirection, &rec->normal) > 0.0;
}

// Dielectric

static inline double Dielectric_Reflectance(const double cosine,
                                            const double refIdx) {
    double r0 = (1 - refIdx) / (1 + refIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

static inline bool Dielectric_Scatter(const material *l, const ray *rayIn,
                                      const hit_record *rec, color *attenuation,
                                      ray *scattered) {
    *attenuation = (color){{1.0, 1.0, 1.0}};
    double refractionRatio = l->fuzz;
    if (rec->frontFace) {
        refractionRatio = 1.0 / l->fuzz;
    }
    const vec3 unitDirection = Vec3_UnitVector(&rayIn->direction);
    const vec3 unitDirNeg = Vec3_Neg(&unitDirection);
    const double cosTheta = fmin(Vec3_Dot(&unitDirNeg, &rec->normal), 1.0);
    const double sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    const bool cannotRefract = refractionRatio * sinTheta > 1.0;

    ray result;
    result.origin = rec->p;

    if (cannotRefract ||
        Dielectric_Reflectance(cosTheta, refractionRatio) > RandomDouble()) {
        result.direction = Vec3_Reflect(&unitDirection, &rec->normal);
    } else {
        result.direction =
            Vec3_Refract(&unitDirection, &rec->normal, refractionRatio);
    }

    *scattered = result;
    return true;
}

static inline bool Mat_Scatter(const material *l, const ray *rayIn,
                               const hit_record *rec, color *attenuation,
                               ray *scattered) {

    switch (l->type) {
    case 0:
        return Lambertian_Scatter(l, rec, attenuation, scattered);
    case 1:
        return Metal_Scatter(l, rayIn, rec, attenuation, scattered);
    case 2:
        return Dielectric_Scatter(l, rayIn, rec, attenuation, scattered);
    default:
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