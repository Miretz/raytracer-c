#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <stdio.h>

typedef vec3 color;

color NewColor(double x, double y, double z) {
    color res = {x, y, z};
    return res;
}

void WriteColor(FILE *fp, color pixelColor, int samplesPerPixel) {
    double scale = 1.0 / samplesPerPixel;
    double r = sqrt(scale * pixelColor.x);
    double g = sqrt(scale * pixelColor.y);
    double b = sqrt(scale * pixelColor.z);
    fprintf(fp, "%d %d %d\n", (int)(256 * Clamp(r, 0.0, 0.999)),
            (int)(256 * Clamp(g, 0.0, 0.999)),
            (int)(256 * Clamp(b, 0.0, 0.999)));
}

#endif