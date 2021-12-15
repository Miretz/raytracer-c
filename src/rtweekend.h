#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <stdlib.h>

const double Pi = 3.1415926535897932385;

static inline double DegreesToRadians(const double degrees) { return degrees * Pi / 180.0; }

static inline double RandomDouble() { return rand() / (RAND_MAX + 1.0); }

static inline double RandomBetween(const double min, const double max) {
    return min + (max - min) * RandomDouble();
}

static inline double Clamp(const double x, const double min, const double max) {
    if (x < min) {
        return min;
    }
    if (x > max) {
        return max;
    }
    return x;
}

#endif