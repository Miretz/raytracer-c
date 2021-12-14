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

camera NewCamera(point3 lookfrom, point3 lookat, vec3 vup, double vFov,
                 double aspectRatio, double aperture, double focusDist) {

    double theta = DegreesToRadians(vFov);
    double h = tan(theta / 2.0);
    double viewportHeight = 2.0 * h;
    double viewportWidth = aspectRatio * viewportHeight;

    camera c;
    vec3 diffLook = Vec3_Sub(&lookfrom, &lookat);
    c.w = Vec3_UnitVector(&diffLook);
    vec3 cross = Vec3_Cross(&vup, &c.w);
    c.u = Vec3_UnitVector(&cross);
    c.v = Vec3_Cross(&c.w, &c.u);
    c.origin = lookfrom;
    c.horizontal = Vec3_FMul(&c.u, viewportWidth * focusDist);
    c.vertical = Vec3_FMul(&c.v, viewportHeight * focusDist);
    c.lowerLeftCorner = Vec3_SubMultiple(
        lookfrom, 3, Vec3_FDiv(&c.horizontal, 2.0), Vec3_FDiv(&c.vertical, 2.0),
        Vec3_FMul(&c.w, focusDist));
    c.lensRadius = aperture / 2;
    return c;
}

ray GetRay(camera *c, double s, double t) {
    vec3 rd = Vec3_RandomInUnitDisk();
    Vec3_MulAssign(&rd, c->lensRadius);
    vec3 offset =
        Vec3_AddMultiple(2, Vec3_FMul(&c->u, rd.x), Vec3_FMul(&c->v, rd.y));
    ray result = {Vec3_Add(&c->origin, &offset),
                  Vec3_AddMultiple(5, &c->lowerLeftCorner,
                                   Vec3_FMul(&c->horizontal, s),
                                   Vec3_FMul(&c->vertical, t),
                                   Vec3_Neg(&c->origin), Vec3_Neg(&offset))};
    return result;
}

#endif