#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <stdio.h>

static inline void WriteColor(FILE *fp, color pixelColor, double colorScale) {
    static const int colorMax = 256;
    static const double colorMaxScaled = 0.999;
    const double r = sqrt(colorScale * pixelColor.x);
    const double g = sqrt(colorScale * pixelColor.y);
    const double b = sqrt(colorScale * pixelColor.z);
    fprintf(fp, "%d %d %d\n", (int)(colorMax * Clamp(r, 0.0, colorMaxScaled)),
            (int)(colorMax * Clamp(g, 0.0, colorMaxScaled)),
            (int)(colorMax * Clamp(b, 0.0, colorMaxScaled)));
}

#endif