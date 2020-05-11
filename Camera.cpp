//
// Created by menegais1 on 28/04/2020.
//

#include <cmath>
#include <iostream>
#include "Camera.h"
#include "Render.h"


Matrix<double> Camera::lookAt(const dvec3 &cameraPos, const dvec3 &cameraPointOfInterest, const dvec3 &up) {
    this->cameraPos = cameraPos;
    this->cameraPointOfInterest = cameraPointOfInterest;
    dvec3 d = (cameraPos - cameraPointOfInterest).unit();
    dvec3 r = up.cross(d).unit();
    dvec3 u = d.cross(r).unit();
    this->cameraDirection = -d;
    Matrix<double> T = Render::translate(dvec3(-cameraPos.x, -cameraPos.y, -cameraPos.z));
    Matrix<double> R(4, 4);
    R[0] = {r.x, r.y, r.z, 0};
    R[1] = {u.x, u.y, u.z, 0};
    R[2] = {d.x, d.y, d.z, 0};
    R[3] = {0, 0, 0, 1};

    View = R * T;
    return View;
}

Matrix<double> Camera::projection(float coefficient) {
    Projection[0] = {1, 0, 0, 0};
    Projection[1] = {0, 1, 0, 0};
    Projection[2] = {0, 0, 1, 0};
    Projection[3] = {0, 0, coefficient, 1};
    return Projection;
}

Matrix<double> Camera::viewport(int x, int y, int w, int h) {
    Viewport[0][3] = x + w / 2.f;
    Viewport[1][3] = y + h / 2.f;
    Viewport[2][3] = Render::depth / 2.f;

    Viewport[0][0] = w / 2.f;
    Viewport[1][1] = h / 2.f;
    Viewport[2][2] = (Render::depth / 2.f) * (1.0 / (cameraPos - cameraPointOfInterest).length());
    return Viewport;
}

Camera::Camera() {
    View = Matrix<double>::identity(4);
    Projection = Matrix<double>::identity(4);
    Viewport = Matrix<double>::identity(4);
}
