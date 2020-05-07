//
// Created by menegais1 on 27/04/2020.
//

#include <iostream>
#include <cmath>
#include "Render.h"
#include "Vectors/Vector3.h"

Render::Render() {
    camera = new Camera();
    image = TGAImage(width, height, TGAImage::RGB);
    depthBuffer = new float[width * height];
    for (int i = 0; i < width * height; ++i) {
        depthBuffer[i] = -100000;
    }
}

dvec3 Render::barycentricCoordinates(dvec3 p0, dvec3 p1, dvec3 p2, dvec3 p) {
    dvec3 v1 = dvec3((p2 - p0).x, (p1 - p0).x, (p0 - p).x);
    dvec3 v2 = dvec3((p2 - p0).y, (p1 - p0).y, (p0 - p).y);
    dvec3 crossProduct = v1.cross(v2);
    if (std::abs(crossProduct.z) < 1) return dvec3(-1, 1, 1);
    return dvec3(1.0 - (crossProduct.x + crossProduct.y) / crossProduct.z, crossProduct.y / crossProduct.z,
                 crossProduct.x / crossProduct.z);
}

dvec3 Render::barycentricCoordinates(dvec2 p0, dvec2 p1, dvec2 p2, dvec2 p) {
    return barycentricCoordinates(dvec3(p0.x, p0.y, 1), dvec3(p1.x, p1.y, 1), dvec3(p2.x, p2.y, 1),
                                  dvec3(p.x, p.y, 1));
}

dvec2 Render::getTriangleMaxBounds(dvec3 p0, dvec3 p1, dvec3 p2) {
    dvec2 max = dvec2(p0.x, p0.y);
    if (p1.x > max.x) max.x = p1.x;
    if (p2.x > max.x) max.x = p2.x;
    if (p1.y > max.y) max.y = p1.y;
    if (p2.y > max.y) max.y = p2.y;
    return max;
}

dvec2 Render::getTriangleMinBounds(dvec3 p0, dvec3 p1, dvec3 p2) {
    dvec2 min = dvec2(p0.x, p0.y);
    if (p1.x < min.x) min.x = p1.x;
    if (p2.x < min.x) min.x = p2.x;
    if (p1.y < min.y) min.y = p1.y;
    if (p2.y < min.y) min.y = p2.y;
    return min;
}

Matrix<double> Render::rotateX(float angle) {
    Matrix<double> R(4, 4);
    R[0] = {1, 0, 0, 0};
    R[1] = {0, cos(angle), -sin(angle), 0};
    R[2] = {0, sin(angle), cos(angle), 0};
    R[3] = {0, 0, 0, 1};
    return R;
}

Matrix<double> Render::rotateY(float angle) {
    Matrix<double> R(4, 4);
    R[0] = {cos(angle), 0, sin(angle), 0};
    R[1] = {0, 1, 0, 0};
    R[2] = {-sin(angle), 0, cos(angle), 0};
    R[3] = {0, 0, 0, 1};
    return R;
}

Matrix<double> Render::rotateZ(float angle) {
    Matrix<double> R(4, 4);
    R[0] = {cos(angle), -sin(angle), 0, 0};
    R[1] = {sin(angle), cos(angle), 0, 0};
    R[2] = {0, 0, 1, 0};
    R[3] = {0, 0, 0, 1};
    return R;
}

Matrix<double> Render::translate(dvec3 translation) {
    Matrix<double> R(4, 4);
    R[0] = {1, 0, 0, translation.x};
    R[1] = {0, 1, 0, translation.y};
    R[2] = {0, 0, 1, translation.z};
    R[3] = {0, 0, 0, 1};
    return R;
}

Matrix<double> Render::rescale(dvec3 scale) {
    Matrix<double> S(4, 4);
    S[0] = {scale.x, 0, 0, 0};
    S[1] = {0, scale.y, 0, 0};
    S[2] = {0, 0, scale.z, 0};
    S[3] = {0, 0, 0, 1};
    return S;
}

Matrix<double> Render::vectorToMatrix(dvec3 vector) {
    Matrix<double> V(4, 1);
    V[0] = {vector.x};
    V[1] = {vector.y};
    V[2] = {vector.z};
    V[3] = {1};
    return V;
}

dvec3 Render::ClipSpaceToNDC(Matrix<double> M) {
    if (M[3][0] == 0) return dvec3(M[0][0], M[1][0], M[2][0]);
    return dvec3(M[0][0] / M[3][0], M[1][0] / M[3][0], M[2][0] / M[3][0]);
}

Matrix<double> Render::viewport(int x, int y, int w, int h) {
    return Matrix<double>();
}

void Render::triangleLineSweep(ivec2 p0, ivec2 p1, ivec2 p2, TGAColor color) {
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

void Render::triangleBarycentric(dvec3 *points, IShader *shader) {
    dvec2 minBounds = getTriangleMinBounds(points[0], points[1], points[2]);
    dvec2 maxBounds = getTriangleMaxBounds(points[0], points[1], points[2]);

    for (int x = minBounds.x; x <= maxBounds.x; ++x) {
        for (int y = minBounds.y; y <= maxBounds.y; ++y) {
            dvec3 p = dvec3(x, y, 0);
            dvec3 barycentricP = barycentricCoordinates(points[0], points[1], points[2], p);
            if (barycentricP.x < 0 || barycentricP.y < 0 || barycentricP.z < 0) continue;
            p.z = barycentricP.x * points[0].z + barycentricP.y * points[1].z + barycentricP.z * points[2].z;
            if (depthBuffer[(int) (y * width + x)] < p.z) {
                TGAColor c;
                bool discard = shader->fragmentShader(barycentricP, c);
                if (discard) return;
                depthBuffer[(int) (y * width + x)] = p.z;
                image.set(x, y, c);
            }
        }
    }
}

void Render::line(dvec3 p0, dvec3 p1, TGAColor color) {
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
        std::swap(p0, p1);
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

Render &Render::getInstance() {
    static Render r;
    return r;
}

Render::~Render() {
    image.flip_vertically();
    image.write_tga_file("output.tga");
}