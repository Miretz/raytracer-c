#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <stdio.h>

static inline void WriteColor(FILE *fp, color pixelColor, double colorScale) {
    static const int colorMax = 256;
    static const double colorMaxScaled = 0.999;
    Vec3_FMulAssign(&pixelColor, colorScale);
    const double r = sqrt(pixelColor.e[0]);
    const double g = sqrt(pixelColor.e[1]);
    const double b = sqrt(pixelColor.e[2]);
    fprintf(fp, "%d %d %d\n", (int)(colorMax * Clamp(r, 0.0, colorMaxScaled)),
            (int)(colorMax * Clamp(g, 0.0, colorMaxScaled)),
            (int)(colorMax * Clamp(b, 0.0, colorMaxScaled)));
}

#endif