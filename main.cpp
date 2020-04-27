#include "tgaimage.h"
#include "Vectors/Vector2.h"
#include "Model.h"
#include "Render.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

int main(int argc, char **argv) {
    TGAImage image(width, height, TGAImage::RGB);
    image.set(30, 30, red);
    image.set(10, 10, red);
    image.set(0, 20, red);
    triangleBarycentric(ivec2(10, 10), ivec2(30, 30), ivec2(0, 20), image, white);
    triangleBarycentric(ivec2(0, 20), ivec2(30, 30), ivec2(0, 40), image, red);
    triangleBarycentric(ivec2(100, 100), ivec2(120, 100), ivec2(200, 200), image, white);
    Model m;
    m.loadObj("../african_head.obj");
    m.renderModel(image, halfWidth, halfHeight, dvec3(0, 0, 1).unit());
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}

