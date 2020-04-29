//
// Created by menegais1 on 28/04/2020.
//

#include <cmath>
#include <iostream>
#include "Camera.h"
#include "Render.h"


Matrix<double>
Camera::setupCameraMatrix(const dvec3 &cameraPos, const dvec3 &cameraPointOfInterest, const dvec3 &upDirection) {
    this->cameraPos = cameraPos;
    this->cameraPointOfInterest = cameraPointOfInterest;
    this->upDirection = upDirection;
    dvec3 d = (cameraPos - cameraPointOfInterest).unit();
    dvec3 r = upDirection.cross(d).unit();
    dvec3 u = d.cross(r).unit();
    Matrix<double> T = translate(dvec3(-cameraPos.x, -cameraPos.y, -cameraPos.z));
    Matrix<double> R(4, 4);
    R[0] = {r.x, r.y, r.z, 0};
    R[1] = {u.x, u.y, u.z, 0};
    R[2] = {d.x, d.y, d.z, 0};
    R[3] = {0, 0, 0, 1};

    CameraMatrix = R * T;
    return CameraMatrix;
}

Matrix<double> Camera::setupProjectionMatrix(float cameraFieldOfView, float near, float far) {
    this->cameraFieldOfView = cameraFieldOfView;
    this->near = near;
    this->far = far;
    ProjectionMatrix[0] = {1.0 / std::tan(cameraFieldOfView / 2), 0, 0, 0};
    ProjectionMatrix[1] = {0, 1.0 / std::tan(cameraFieldOfView / 2), 0, 0};
    ProjectionMatrix[2] = {0, 0, (-2.0 * far * near) / (far - near), (-(far + near)) / (far - near)};
    ProjectionMatrix[3] = {0, 0, -1, 0};
    return ProjectionMatrix;
}

Camera::Camera() {
    CameraMatrix = Matrix<double>(4, 4);
    ProjectionMatrix = Matrix<double>(4, 4);
}
