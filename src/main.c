#define _CRT_SECURE_NO_DEPRECATE
#include <pthread.h>
#include <stdio.h>

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

color Ray_Color(const ray *r, hittable_list *world, const int depth) {
    if (depth < 0) {
        return (color){{0.0, 0.0, 0.0}};
    }
    hit_record rec = {};
    if (Hittable_Hit(world, r, 0.001, 99999.0, &rec)) {
        ray scattered;
        color attenuation;
        if (Mat_Scatter(rec.matPtr, r, &rec, &attenuation, &scattered)) {
            vec3 rc = Ray_Color(&scattered, world, depth - 1);
            Vec3_MulAssign(&rc, &attenuation);
            return rc;
        }
    }
    const vec3 unitDirection = Vec3_UnitVector(&r->direction);
    double t = 0.5 * (unitDirection.e[1] + 1.0);
    static const color bgColor1 = {{1.0, 1.0, 1.0}};
    static const color bgColor2 = {{0.5, 0.7, 1.0}};

    vec3 r1 = Vec3_FMul(&bgColor1, (1.0 - t));
    const vec3 r2 = Vec3_FMul(&bgColor2, t);
    Vec3_AddAssign(&r1, &r2);
    return r1;
}

hittable_list *randomScene() {
    hittable_list *world = NewHittableList();

    const material groundMaterial = NewMaterial(0, (color){{0.5, 0.5, 0.5}}, 0.0);
    const sphere ground =
        NewSphere((point3){{0, -1000, 0}}, 1000, groundMaterial);
    Hittable_Add(world, ground);

    static const point3 maxDist = {{4, 0.2, 0}};

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            const double chooseMat = RandomDouble();
            const point3 center = {{(double)a + 0.9 * RandomDouble(), 0.2,
                                   (double)b + 0.9 * RandomDouble()}};

            const vec3 subPoint = Vec3_Sub(&center, &maxDist);
            if (Vec3_Length(&subPoint) > 0.9) {
                material sphereMaterial;

                if (chooseMat < 0.8) {
                    // diffuse
                    const vec3 r1 = Vec3_Random();
                    const vec3 r2 = Vec3_Random();
                    const color albedo = Vec3_Mul(&r1, &r2);
                    sphereMaterial = NewMaterial(0, albedo, 0.0);

                } else if (chooseMat < 0.95) {
                    // metal
                    const color albedo = Vec3_RandomBetween(0.5, 1);
                    const double fuzz = RandomBetween(0, 0.5);
                    sphereMaterial = NewMaterial(1, albedo, fuzz);
                } else {
                    // glass
                    sphereMaterial =
                        NewMaterial(2, (color){{1.0, 1.0, 1.0}}, 1.5);
                }

                Hittable_Add(world, NewSphere(center, 0.2, sphereMaterial));
            }
        }
    }

    const material material1 = NewMaterial(2, (color){{1.0, 1.0, 1.0}}, 1.5);
    const material material2 = NewMaterial(0, (color){{0.4, 0.2, 0.1}}, 0.0);
    const material material3 = NewMaterial(1, (color){{0.7, 0.6, 0.5}}, 0.0);

    const sphere s1 = NewSphere((point3){{0, 1, 0}}, 1.0, material1);
    const sphere s2 = NewSphere((point3){{-4, 1, 0}}, 1.0, material2);
    const sphere s3 = NewSphere((point3){{4, 1, 0}}, 1.0, material3);

    Hittable_Add(world, s1);
    Hittable_Add(world, s2);
    Hittable_Add(world, s3);

    return world;
}

typedef struct thread_input {
    int startRow;
    int stopRow;
    int imageWidth;
    int imageHeight;
    int samplesPerPixel;
    int maxDepth;
    camera *cam;
    hittable_list *world;
} thread_input;

void *renderPixel(void *arg) {
    thread_input *tdata = (thread_input *)arg;

    const int width = tdata->imageWidth;
    const int height = tdata->imageHeight;
    const int samplesPerPixel = tdata->samplesPerPixel;
    const int maxDepth = tdata->maxDepth;
    const int start = tdata->startRow;
    const int stop = tdata->stopRow;

    register int j = start;
    register int i = 0;
    register int s = 0;

    color *outData = (color *)malloc(((stop - start) * width) * sizeof(color));
    int index = 0;
    for (j = start; j < stop; ++j) {
        for (i = 0; i < width; ++i) {
            color pixelColor = (color){{0.0, 0.0, 0.0}};
            for (s = samplesPerPixel; s; s--) {
                const double u = (i + RandomDouble()) / (width - 1);
                const double v = (j + RandomDouble()) / (height - 1);
                const ray r = GetRay(tdata->cam, u, v);
                const vec3 rayColor = Ray_Color(&r, tdata->world, maxDepth);
                Vec3_AddAssign(&pixelColor, &rayColor);
            }
            outData[index] = pixelColor;
            index++;
        }
    }
    pthread_exit(outData);
    return outData;
}

void Render() {

    // Settings
    const double aspectRatio = 3.0 / 2.0;
    const int imageWidth = 600;
    const int imageHeight = (int)(imageWidth / aspectRatio);
    const int samplesPerPixel = 100;
    const double colorScale = 1.0 / samplesPerPixel;
    const int maxDepth = 50;

    // Create World
    hittable_list *world = randomScene();

    // Camera
    const vec3 lookfrom = {{13, 2, 3}};
    const vec3 lookat = {{0, 0, 0}};
    const vec3 vup = {{0, 1, 0}};
    const double distToFocus = 10.0;
    const double aperture = 0.1;
    camera *cam = NewCamera(lookfrom, lookat, vup, 20, aspectRatio, aperture,
                            distToFocus);

    // Multi-threaded rendering
    const int threadCount = 100;
    pthread_t tid[threadCount];
    thread_input td[threadCount];
    for (int t = 0; t < threadCount; ++t) {

        td[t].imageWidth = imageWidth;
        td[t].imageHeight = imageHeight;
        td[t].samplesPerPixel = samplesPerPixel;
        td[t].maxDepth = maxDepth;

        td[t].startRow = t * (imageHeight / threadCount);
        td[t].stopRow = (t + 1) * (imageHeight / threadCount);
        if (td[t].stopRow > (imageWidth * imageHeight)) {
            td[t].stopRow = imageWidth * imageHeight;
        }
        td[t].cam = cam;
        td[t].world = world;
        printf("Thread with range (%d, %d) started\n", td[t].startRow,
               td[t].stopRow);
        pthread_create(&(tid[t]), NULL, renderPixel, (void *)&(td[t]));
    }

    color *pixelColorArray =
        (color *)malloc(sizeof(color) * imageWidth * imageHeight);

    // collect the results
    for (int t = 0; t < threadCount; ++t) {
        color *temp = NULL;
        pthread_join(tid[t], (void **)&temp);

        const int start = td[t].startRow;
        const int stop = td[t].stopRow;

        int index = 0;
        for (int j = start; j < stop; ++j) {
            for (int i = 0; i < imageWidth; ++i) {
                const int invJ = imageHeight - j - 1;
                const int imageIndex = imageWidth * invJ + i;
                pixelColorArray[imageIndex] = temp[index];
                index++;
            }
        }

        if (temp != NULL) {
            free(temp);
        }
    }

    // output to file
    FILE *file = NULL;
    if ((file = fopen("output.ppm", "w+")) == NULL) {
        perror("fopen");
        exit(1);
    }
    fprintf(file, "P3\n%d %d\n255\n", imageWidth, imageHeight);
    for (int i = 0; i < imageWidth * imageHeight; ++i) {
        WriteColor(file, pixelColorArray[i], colorScale);
    }
    fclose(file);

    free(pixelColorArray);
    free(cam);
    free(world);
}

int main() {
    Render();
    return 0;
}
