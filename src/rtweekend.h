#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <stdlib.h>

const double Pi = 3.1415926535897932385;

double DegreesToRadians(double degrees) { return degrees * Pi / 180.0; }

double RandomDouble() { return rand() / (RAND_MAX + 1.0); }

double RandomBetween(double min, double max) {
    return min + (max - min) * RandomDouble();
}

double Clamp(double x, double min, double max) {
    if (x < min) {
        return min;
    }
    if (x > max) {
        return max;
    }
    return x;
}

#endif