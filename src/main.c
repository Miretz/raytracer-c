#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

const double aspectRatio = 3.0 / 2.0;
const int imageWidth = 600;
const int imageHeight = (int)(imageWidth / aspectRatio);
const int samplesPerPixel = 50;
const int maxDepth = 50;

color Ray_Color(ray *r, hittable_list *world, int depth) {
    if (depth < 0) {
        return NewColor(0, 0, 0);
    }
    hit_record rec;
    if (Hittable_Hit(world, r, 0.001, 9999.0, &rec)) {
        ray scattered;
        color attenuation;
        if (Mat_Scatter(&rec.mat, r, &rec, &attenuation, &scattered)) {
            vec3 rc = Ray_Color(&scattered, world, depth - 1);
            return Vec3_Mul(&attenuation, &rc);
        }
    }
    vec3 unitDirection = Vec3_UnitVector(&r->direction);
    double t = 0.5 * (unitDirection.y + 1.0);
    color bgColor1 = {1.0, 1.0, 1.0};
    color bgColor2 = {0.5, 0.7, 1.0};
    vec3 r1 = Vec3_FMul(&bgColor1, (1.0 - t));
    vec3 r2 = Vec3_FMul(&bgColor2, t);
    Vec3_AddAssign(&r1, &r2);
    return r1;
}

hittable_list randomScene() {
    hittable_list world = NewHittableList();

    material groundMaterial = NewMaterial(0, NewColor(0.5, 0.5, 0.5), 0.0);
    sphere ground = NewSphere(Point3(0, -1000, 0), 1000, groundMaterial);
    Hittable_Add(&world, ground);

    point3 maxDist = Point3(4, 0.2, 0);

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double chooseMat = RandomDouble();
            point3 center = Point3((double)a + 0.9 * RandomDouble(), 0.2,
                                   (double)b + 0.9 * RandomDouble());

            vec3 subPoint = Vec3_Sub(&center, &maxDist);
            if (Vec3_Length(&subPoint) > 0.9) {
                material sphereMaterial;

                if (chooseMat < 0.8) {
                    // diffuse
                    vec3 r1 = Vec3_Random();
                    vec3 r2 = Vec3_Random();
                    color albedo = Vec3_Mul(&r1, &r2);
                    sphereMaterial = NewMaterial(0, albedo, 0.0);
                    Hittable_Add(&world,
                                 NewSphere(center, 0.2, sphereMaterial));
                } else if (chooseMat < 0.95) {
                    // metal
                    color albedo = Vec3_RandomBetween(0.5, 1);
                    double fuzz = RandomBetween(0, 0.5);
                    sphereMaterial = NewMaterial(1, albedo, fuzz);
                    Hittable_Add(&world,
                                 NewSphere(center, 0.2, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial =
                        NewMaterial(2, NewColor(1.0, 1.0, 1.0), 1.5);
                    Hittable_Add(&world,
                                 NewSphere(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    material material1 = NewMaterial(2, NewColor(1.0, 1.0, 1.0), 1.5);
    material material2 = NewMaterial(0, NewColor(0.4, 0.2, 0.1), 0.0);
    material material3 = NewMaterial(1, NewColor(0.7, 0.6, 0.5), 0.0);

    sphere s1 = NewSphere(Point3(0, 1, 0), 1.0, material1);
    sphere s2 = NewSphere(Point3(-4, 1, 0), 1.0, material2);
    sphere s3 = NewSphere(Point3(4, 1, 0), 1.0, material3);

    Hittable_Add(&world, s1);
    Hittable_Add(&world, s2);
    Hittable_Add(&world, s3);

    return world;
}

void Render() {

    // Create World
    hittable_list world = randomScene();

    // Camera
    vec3 lookfrom = {13, 2, 3};
    vec3 lookat = {0, 0, 0};
    vec3 vup = {0, 1, 0};
    double distToFocus = 10.0;
    double aperture = 0.1;
    camera cam = NewCamera(lookfrom, lookat, vup, 20, aspectRatio, aperture,
                           distToFocus);

    FILE *file;
    if ((file = fopen("output.ppm", "w+")) == NULL) {
        perror("fopen");
        exit(1);
    }

    fprintf(file, "P3\n%d %d\n255\n", imageWidth, imageHeight);
    for (int j = imageHeight - 1; j >= 0; --j) {
        fprintf(stderr, "\rScanlines remaining: %d  ", j);
        for (int i = 0; i < imageWidth; ++i) {
            color pixelColor = NewColor(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                double u = (i + RandomDouble()) / (imageWidth - 1);
                double v = (j + RandomDouble()) / (imageHeight - 1);
                ray r = GetRay(&cam, u, v);
                vec3 rayColor = Ray_Color(&r, &world, maxDepth);
                Vec3_AddAssign(&pixelColor, &rayColor);
            }
            WriteColor(file, pixelColor, samplesPerPixel);
        }
    }

    fclose(file);
}

int main() {
    Render();
    return 0;
}
