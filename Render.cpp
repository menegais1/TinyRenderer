//
// Created by menegais1 on 27/04/2020.
//

#include <iostream>
#include "Render.h"
#include "Vectors/Vector3.h"

void line(ivec2 p0, ivec2 p1, TGAImage &image, TGAColor color) {
    int height = abs(p1.y - p0.y);
    int width = abs(p1.x - p0.x);
    bool transposed = false;
    //Transpose the line if the height is greater than the width, to not make changes to the for
    //bellow, we could just make two different fors
    if (height > width) {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        transposed = true;
    }

    //Guarantee that the line is always left to right
    if (p0.x > p1.x) {
        std::swap(p0.x, p1.x);
        std::swap(p0.y, p1.y);
    }

    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;
    //Always less than 1
    int dError2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = p0.y;
    for (int x = p0.x; x <= p1.x; x++) {
        //transpose back
        if (transposed)
            image.set(y, x, color);
        else
            image.set(x, y, color);

        error2 += dError2;
        //if error is greather than 0.5, it means it is in another y pixel, as error is the distance between the current y and the line
        //The dx bit and the 2 times error and derror part are a way to get only integer math, so we are just
        //keeping the proportions integer, instead of float
        if (error2 > dx) {
            y += (p1.y > p0.y ? 1 : -1);
            //This sets the distance to the bottom part of the current y pixel
            error2 -= dx * 2;
        }
    }
}

//Line sweep triangle generation algorithm
void triangleLineSweep(ivec2 p0, ivec2 p1, ivec2 p2, TGAImage &image, TGAColor color) {
    if (p0 == p1 || p1 == p2 || p0 == p2) return;
    if (p0.y > p1.y) std::swap(p0, p1);
    if (p0.y > p2.y) std::swap(p0, p2);
    if (p1.y > p2.y) std::swap(p1, p2);
    float betha, alpha;
    ivec2 p01, p02;
    bool upperHalf = false;

    ivec2 v1 = p1 - p0;
    ivec2 v2 = p2 - p0;
    ivec2 v3 = p2 - p1;
    for (int y = p0.y; y <= p2.y; ++y) {
        if (y > p1.y) upperHalf = true;
        alpha = (y - p0.y) / (float) v2.y;
        if (!upperHalf) {
            betha = (y - p0.y) / ((float) v1.y + 1);
            p01 = p0 + ivec2(v1.x * betha, v1.y * betha);
        } else {
            betha = (y - p1.y) / ((float) v3.y + 1);
            p01 = p1 + ivec2(v3.x * betha, v3.y * betha);
        }
        p02 = p0 + ivec2(v2.x * alpha, v2.y * alpha);
        if (p01.x > p02.x)std::swap(p01, p02);
        for (int x = p01.x; x <= p02.x; ++x) {
            image.set(x, y, color);
        }
    }
}


dvec3 barycentricCoordinates(dvec3 p0, dvec3 p1, dvec3 p2, dvec3 p) {
    dvec3 v1 = dvec3((p2 - p0).x, (p1 - p0).x, (p0 - p).x);
    dvec3 v2 = dvec3((p2 - p0).y, (p1 - p0).y, (p0 - p).y);
    dvec3 crossProduct = v1.cross(v2);
    if (std::abs(crossProduct.z) < 1) return dvec3(-1, 1, 1);
    return dvec3(1.0 - (crossProduct.x + crossProduct.y) / crossProduct.z, crossProduct.y / crossProduct.z,
                 crossProduct.x / crossProduct.z);
}

dvec2 getTriangleMaxBounds(dvec3 p0, dvec3 p1, dvec3 p2) {
    dvec2 max = dvec2(p0.x, p0.y);
    if (p1.x > max.x) max.x = p1.x;
    if (p2.x > max.x) max.x = p2.x;
    if (p1.y > max.y) max.y = p1.y;
    if (p2.y > max.y) max.y = p2.y;
    return max;
}

dvec2 getTriangleMinBounds(dvec3 p0, dvec3 p1, dvec3 p2) {
    dvec2 min = dvec2(p0.x, p0.y);
    if (p1.x < min.x) min.x = p1.x;
    if (p2.x < min.x) min.x = p2.x;
    if (p1.y < min.y) min.y = p1.y;
    if (p2.y < min.y) min.y = p2.y;
    return min;
}

//Barycentric coordinates triangle generation algorithm
void triangleBarycentric(dvec3 p0, dvec3 p1, dvec3 p2, float *zBuffer, TGAImage &image, TGAColor color) {
    dvec2 minBounds = getTriangleMinBounds(p0, p1, p2);
    dvec2 maxBounds = getTriangleMaxBounds(p0, p1, p2);

    for (int x = minBounds.x; x <= maxBounds.x; ++x) {
        for (int y = minBounds.y; y <= maxBounds.y; ++y) {

            dvec3 p = dvec3(x, y, 1);
            dvec3 barycentricP = barycentricCoordinates(p0, p1, p2, p);
            if (barycentricP.x < 0 || barycentricP.y < 0 || barycentricP.z < 0) continue;
            p.z = barycentricP.x * p0.z + barycentricP.y * p1.z + barycentricP.z * p2.z;
            if (zBuffer[y * width + x] < p.z) {
                zBuffer[y * width + x] = p.z;
                image.set(x, y, color);
            }
        }
    }
}