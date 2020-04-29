//
// Created by menegais1 on 28/04/2020.
//

#include <cmath>
#include <iostream>
#include "Camera.h"
#include "Render.h"

Camera::Camera(const dvec3 &cameraPos, const dvec3 &cameraPointOfInterest, const dvec3 &upDirection,
               float cameraFieldOfView, float near, float far) :
        cameraPos(cameraPos),
        cameraPointOfInterest(cameraPointOfInterest),
        upDirection(upDirection),
        cameraFieldOfView(cameraFieldOfView), near(near),
        far(far) {
    CameraMatrix = Matrix<double>(4, 4);
    PerspectiveMatrix = Matrix<double>(4, 4);
    setupCameraMatrix();
    setupPerspectiveMatrix();
}

void Camera::setupCameraMatrix() {

}

void Camera::setupPerspectiveMatrix() {

}

