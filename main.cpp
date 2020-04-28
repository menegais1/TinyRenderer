#include "tgaimage.h"
#include "Vectors/Vector2.h"
#include "Model.h"
#include "Render.h"
#include "Vectors/Matrix.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

int main(int argc, char **argv) {
    TGAImage image(width, height, TGAImage::RGB);
    Model m;
    m.loadObj("../african_head.obj");
    m.renderModel(image, halfWidth, halfHeight, dvec3(0, 0, -1).unit());
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}

