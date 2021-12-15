#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "sphere.h"

#define MAX_OBJECTS 10000

typedef struct hittable_list {
    int count;
    sphere objects[];
} hittable_list;

static inline unsigned int Hittable_Hit(const hittable_list *hl, const ray *r,
                               const double tMin, const double tMax,
                               hit_record *rec) {
    hit_record tempRec;
    int hitAnything = 0;
    double closestSoFar = tMax;
    for (int i = 0; i < hl->count; ++i) {
        if (Sphere_Hit(&(hl->objects[i]), r, tMin, closestSoFar, &tempRec)) {
            hitAnything = 1;
            closestSoFar = tempRec.t;
            *rec = tempRec;
        }
    }
    return hitAnything;
}

static inline hittable_list *NewHittableList() {
    hittable_list *hl =
        malloc(sizeof(hittable_list) + MAX_OBJECTS * sizeof(sphere));
    hl->count = 0;
    return hl;
}

static inline void Hittable_Add(hittable_list *hl, sphere object) {
    hl->objects[hl->count] = object;
    hl->count++;
}

#endif