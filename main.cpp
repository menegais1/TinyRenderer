#include "tgaimage.h"
#include "Vectors/Vector2.h"
#include "Model.h"
#include "Render.h"
#include "Vectors/Matrix.h"
#include "Camera.h"
#include <iostream>
#include <cmath>

const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
TGAImage image(width, height, TGAImage::RGB);

void loadModelAndRender() {
    Model m;
    m.loadObj("../african_head.obj");
    m.renderModel(image, halfWidth, halfHeight, dvec3(0, 0, -1).unit());
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
}

int main(int argc, char **argv) {
    Camera c;
    c.setupCameraMatrix(dvec3(1, 1, 1), dvec3(0, 0, 0), dvec3(0, 1, 0));
 //   c.setupCameraMatrix(dvec3(0, 0, 0), dvec3(0, 0, -1), dvec3(0, 1, 0));
    c.setupProjectionMatrix(3.145 / 2.0, 0.1, 1000);
    dvec3 p0 = dvec3(0.1 * std::tan(3.145 / 4.0), 0.1 * std::tan(3.145 / 4.0), -0.1);
   // dvec3 p2 = dvec3(1000 * std::tan(3.145 / 4.0), 1000 * std::tan(3.145 / 4.0), -1000) * 0.5;
    dvec3 p1 = dvec3(0, 0, -1000);
    p0 = convertPointToScreenSpace(p0, c.CameraMatrix, c.ProjectionMatrix);
    p1 = convertPointToScreenSpace(p1, c.CameraMatrix, c.ProjectionMatrix);
   // p2 = convertPointToScreenSpace(p2, c.CameraMatrix, c.ProjectionMatrix);
    p0.print();
    p1.print();
    //p2.print();
    p0 = dvec3(0, 0, 0);
     p1 = dvec3(5, 0, 0);
    dvec3 p3 = dvec3(0, 5, 0);
    dvec3 p5 = dvec3(0, 0, -5);
    p0 = convertPointToScreenSpace(p0, c.CameraMatrix, c.ProjectionMatrix);
    p1 = convertPointToScreenSpace(p1, c.CameraMatrix, c.ProjectionMatrix);
    p3 = convertPointToScreenSpace(p3, c.CameraMatrix, c.ProjectionMatrix);
    p5 = convertPointToScreenSpace(p5, c.CameraMatrix, c.ProjectionMatrix);
    p0.print();
    p1.print();
    p3.print();
    p5.print();
    p0 = dvec3((p0.x + 1) * halfWidth, (p0.y + 1) * halfHeight, p0.z);
    p1 = dvec3((p1.x + 1) * halfWidth, (p1.y + 1) * halfHeight, p1.z);
    p3 = dvec3((p3.x + 1) * halfWidth, (p3.y + 1) * halfHeight, p3.z);
    p5 = dvec3((p5.x + 1) * halfWidth, (p5.y + 1) * halfHeight, p5.z);
    line(p0, p1, nullptr, image, blue);
    line(p0, p3, nullptr, image, red);
    line(p0, p5, nullptr, image, green);
    //image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}

