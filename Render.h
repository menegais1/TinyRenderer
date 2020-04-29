//
// Created by menegais1 on 27/04/2020.
//

#ifndef TINYRENDERER_RENDER_H
#define TINYRENDERER_RENDER_H


#include "Vectors/Vector2.h"
#include "tgaimage.h"
#include "Vectors/Vector3.h"
#include "Vectors/Matrix.h"

const int width = 1200;
const int height = 1200;
const float halfWidth = width / 2.0;
const float halfHeight = height / 2.0;

Matrix<double> rotateX(float angle);

Matrix<double> rotateY(float angle);

Matrix<double> rotateZ(float angle);

Matrix<double> translate(dvec3 translation);

Matrix<double> rescale(dvec3 scale);

Matrix<double> vectorToMatrix(dvec3 vector);

void initializeCameraMatrix(dvec3 cameraPos, dvec3 cameraPointOfInterest, dvec3 cameraUp,
                            float cameraFieldOfView, float near, float far);

dvec3 convertPointToScreenSpace(dvec3 p, Matrix<double> V, Matrix<double> P);

dvec3 convertPointToViewSpace(dvec3 p, Matrix<double> V);

Matrix<double> convertPointToClipSpace(dvec3 p, Matrix<double> P);

//Line sweep triangle generation algorithm
void triangleLineSweep(ivec2 p0, ivec2 p1, ivec2 p2, TGAImage &image, TGAColor color);

//Barycentric coordinates triangle generation algorithm
void triangleBarycentric(dvec3 p0, dvec3 p1, dvec3 p2, float *zBuffer, TGAImage &image, TGAColor color);

void triangleBarycentric(dvec3 p0, dvec3 p1, dvec3 p2, float *zBuffer, TGAImage &image, TGAImage &texture, dvec2 uv0,
                         dvec2 uv1, dvec2 uv2, float lightIntensity);

//Bresenham line algorithm implementation
//Extra explanation: https://www.youtube.com/watch?v=IDFB5CDpLDE
void line(dvec3 p0, dvec3 p1, float *zBuffer, TGAImage &image, TGAColor color);

float *initializeZBuffer();

dvec3 barycentricCoordinates(dvec3 p0, dvec3 p1, dvec3 p2, dvec3 p);

dvec3 barycentricCoordinates(dvec2 p0, dvec2 p1, dvec2 p2, dvec2 p);

dvec2 getTriangleMinBounds(dvec3 p0, dvec3 p1, dvec3 p2);

dvec2 getTriangleMaxBounds(dvec3 p0, dvec3 p1, dvec3 p2);

#endif //TINYRENDERER_RENDER_H
