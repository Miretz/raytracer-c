#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <stdbool.h>

#include "hittable.h"
#include "sphere.h"

#define MAX_OBJECTS 1000

typedef struct hittable_list {
    int count;
    sphere objects[];
} hittable_list;

static inline bool Hittable_Hit(hittable_list *hl, const ray *r,
                                        const double tMin, const double tMax,
                                        hit_record *rec) {
    hit_record tempRec;
    bool hitAnything = false;
    double closestSoFar = tMax;

    register int i = hl->count + 1;
    for (i = hl->count + 1; i--;) {
        if (Sphere_Hit(&(hl->objects[i - 1]), r, tMin, closestSoFar,
                       &tempRec)) {
            hitAnything = true;
            closestSoFar = tempRec.t;
            *rec = tempRec;
        }
    }
    return hitAnything;
}

static inline hittable_list *NewHittableList() {
    hittable_list *hl = (hittable_list *)malloc(sizeof(hittable_list) +
                                                MAX_OBJECTS * sizeof(sphere));
    hl->count = 0;
    return hl;
}

static inline void Hittable_Add(hittable_list *hl, sphere object) {
    hl->objects[hl->count] = object;
    hl->count++;
}

#endif