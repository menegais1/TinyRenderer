//
// Created by menegais1 on 28/04/2020.
//

#ifndef TINYRENDERER_CAMERA_H
#define TINYRENDERER_CAMERA_H


#include "Vectors/Vector3.h"
#include "Vectors/Matrix.h"

class Camera {
public:
    Matrix<double> View;
    Matrix<double> Projection;
    Matrix<double> Viewport;
    dvec3 cameraPos;
    dvec3 cameraPointOfInterest;
    dvec3 cameraDirection;

    Camera();

    Matrix<double> lookAt(const dvec3 &cameraPos, const dvec3 &cameraPointOfInterest, const dvec3 &up);

    Matrix<double> projection(float coefficient);

    Matrix<double> viewport(int x, int y, int w, int h);
};


#endif //TINYRENDERER_CAMERA_H
