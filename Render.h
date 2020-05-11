//
// Created by menegais1 on 27/04/2020.
//

#ifndef TINYRENDERER_RENDER_H
#define TINYRENDERER_RENDER_H


#include "Vectors/Vector2.h"
#include "tgaimage.h"
#include "Vectors/Vector3.h"
#include "Vectors/Matrix.h"
#include "Camera.h"
#include "IShader.h"


class Render {
public:
    static const int width = 1200;
    static const int height = 1200;
    static const int depth = 255;

    TGAImage image;
    float *depthBuffer;
    float *shadowBuffer;
    Matrix<double> worldToShadowMap;

    Camera *camera;

    static Render &getInstance();

    static Matrix<double> rotateX(float angle);

    static Matrix<double> rotateY(float angle);

    static Matrix<double> rotateZ(float angle);

    static Matrix<double> translate(dvec3 translation);

    static Matrix<double> rescale(dvec3 scale);

    static Matrix<double> vectorToMatrix(dvec3 vector);

    static dvec3 ClipSpaceToNDC(Matrix<double> M);

    Matrix<double> viewport(int x, int y, int w, int h);

    //Triangle rasterization: http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
    //Line sweep triangle generation algorithm
    void triangleLineSweep(ivec2 p0, ivec2 p1, ivec2 p2, TGAColor color);

    void triangleBarycentric(dvec3 *points, IShader *shader, TGAImage image, float *depthBuffer);

    //Bresenham line algorithm implementation
    //Extra explanation: https://www.youtube.com/watch?v=IDFB5CDpLDE
    void line(dvec3 p0, dvec3 p1, TGAColor color);

    ~Render();

private:
    dvec3 barycentricCoordinates(dvec3 p0, dvec3 p1, dvec3 p2, dvec3 p);

    dvec3 barycentricCoordinates(dvec2 p0, dvec2 p1, dvec2 p2, dvec2 p);

    dvec2 getTriangleMinBounds(dvec3 p0, dvec3 p1, dvec3 p2);

    dvec2 getTriangleMaxBounds(dvec3 p0, dvec3 p1, dvec3 p2);

    Render();
};

#endif //TINYRENDERER_RENDER_H
