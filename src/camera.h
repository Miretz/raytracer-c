#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include <stdlib.h>

#include "ray.h"
#include "vec3.h"

typedef struct camera {
    point3 origin;
    point3 lowerLeftCorner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u;
    vec3 v;
    vec3 w;
    double lensRadius;
} camera;

static inline camera *NewCamera(const point3 lookfrom, const point3 lookat,
                                const vec3 vup, const double vFov,
                                const double aspectRatio, const double aperture,
                                const double focusDist) {

    const double viewportHeight = 2.0 * tan(DegreesToRadians(vFov) / 2.0);
    const double viewportWidth = aspectRatio * viewportHeight;

    camera *c = malloc(sizeof(camera));

    const vec3 diffLook = Vec3_Sub(&lookfrom, &lookat);
    c->w = Vec3_UnitVector(&diffLook);
    const vec3 cross = Vec3_Cross(&vup, &c->w);
    c->u = Vec3_UnitVector(&cross);
    c->v = Vec3_Cross(&c->w, &c->u);
    c->origin = lookfrom;
    c->horizontal = Vec3_FMul(&c->u, viewportWidth * focusDist);
    c->vertical = Vec3_FMul(&c->v, viewportHeight * focusDist);
    
    vec3 llcResult = lookfrom;
    vec3 halfH = Vec3_FDiv(&c->horizontal, 2.0);
    vec3 halfV = Vec3_FDiv(&c->vertical, 2.0);
    vec3 focusV = Vec3_FMul(&c->w, focusDist);
    Vec3_SubAssign(&llcResult, &halfH);
    Vec3_SubAssign(&llcResult, &halfV);
    Vec3_SubAssign(&llcResult, &focusV);
    c->lowerLeftCorner = llcResult;
    
    c->lensRadius = aperture / 2.0;
    return c;
}

static inline ray GetRay(camera *c, double s, double t) {
    const vec3 rd = Vec3_RandomInUnitDisk(c->lensRadius);
    vec3 offset = Vec3_FMul(&c->u, rd.e[0]);
    const vec3 offsetV = Vec3_FMul(&c->v, rd.e[1]);
    Vec3_AddAssign(&offset, &offsetV);
    return (ray){Vec3_Add(&c->origin, &offset),
                 Vec3_Add5(c->lowerLeftCorner, Vec3_FMul(&c->horizontal, s),
                           Vec3_FMul(&c->vertical, t), Vec3_Neg(&c->origin),
                           Vec3_Neg(&offset))};
}

#endif