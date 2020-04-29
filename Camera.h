//
// Created by menegais1 on 28/04/2020.
//

#ifndef TINYRENDERER_CAMERA_H
#define TINYRENDERER_CAMERA_H


#include "Vectors/Vector3.h"
#include "Vectors/Matrix.h"

class Camera {
public:
    Matrix<double> CameraMatrix;
    Matrix<double> PerspectiveMatrix;
    dvec3 cameraPos;
    dvec3 cameraPointOfInterest;
    dvec3 upDirection;
    float cameraFieldOfView;
    float near;
    float far;

    Camera(const dvec3 &cameraPos, const dvec3 &cameraPointOfInterest, const dvec3 &upDirection, float cameraFieldOfView,
           float near, float far);

    void setupCameraMatrix();
    void setupPerspectiveMatrix();
};


#endif //TINYRENDERER_CAMERA_H
