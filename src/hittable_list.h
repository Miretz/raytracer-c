#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "sphere.h"

#define MAX_OBJECTS 1000

typedef struct hittable_list {
    int count;
    sphere objects[MAX_OBJECTS];
} hittable_list;

int Hittable_Hit(hittable_list *hl, ray *r, double tMin, double tMax,
                 hit_record *rec) {
    hit_record tempRec;
    int hitAnything = 0;
    double closestSoFar = tMax;
    for (int i = 0; i < hl->count; ++i) {
        sphere sp = hl->objects[i];
        if (Sphere_Hit(&sp, r, tMin, closestSoFar, &tempRec)) {
            hitAnything = 1;
            closestSoFar = tempRec.t;
            *rec = tempRec;
        }
    }
    return hitAnything;
}

hittable_list NewHittableList() {
    hittable_list hl;
    hl.count = 0;
    return hl;
}

void Hittable_Add(hittable_list *hl, sphere object) {
    hl->objects[hl->count] = object;
    hl->count++;
}

#endif