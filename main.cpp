#include "tgaimage.h"
#include "Vectors/Vector2.h"
#include "Model.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

//Bresenham line algorithm implementation
//Extra explanation: https://www.youtube.com/watch?v=IDFB5CDpLDE
void line(ivec2 p0, ivec2 p1, TGAImage &image, TGAColor color) {
    int height = abs(p1.y - p0.y);
    int width = abs(p1.x - p0.x);
    bool transposed = false;
    //Transpose the line if the height is greater than the width, to not make changes to the for
    //bellow, we could just make two different fors
    if (height > width) {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        transposed = true;
    }

    //Guarantee that the line is always left to right
    if (p0.x > p1.x) {
        std::swap(p0.x, p1.x);
        std::swap(p0.y, p1.y);
    }

    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;
    //Always less than 1
    int dError2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = p0.y;
    for (int x = p0.x; x <= p1.x; x++) {
        //transpose back
        if (transposed)
            image.set(y, x, color);
        else
            image.set(x, y, color);

        error2 += dError2;
        //if error is greather than 0.5, it means it is in another y pixel, as error is the distance between the current y and the line
        //The dx bit and the 2 times error and derror part are a way to get only integer math, so we are just
        //keeping the proportions integer, instead of float
        if (error2 > dx) {
            y += (p1.y > p0.y ? 1 : -1);
            //This sets the distance to the bottom part of the current y pixel
            error2 -= dx * 2;
        }
    }
}

void triangle(ivec2 p0, ivec2 p1, ivec2 p2, TGAImage &image, TGAColor color) {
    if (p0.y > p1.y) std::swap(p0, p1);
    if (p0.y > p2.y) std::swap(p0, p2);
    if (p1.y > p2.y) std::swap(p1, p2);

    ivec2 v1 = p1 - p0;
    ivec2 v2 = p2 - p0;
    ivec2 v3 = p2 - p1;
    for (int y = p0.y; y <= p1.y; ++y) {
        float alpha = (y - p0.y) / (float) v2.y ;
        float betha = (y - p0.y) / ((float) v1.y + 1);
        ivec2 p01 = p0 + ivec2(v1.x * betha,v1.y * betha);
        ivec2 p02 = p0 + ivec2(v2.x * alpha,v2.y * alpha);
        if (p01.x > p02.x)std::swap(p01, p02);
        for (int x = p01.x; x <= p02.x; ++x) {
            image.set(x, y, color);
        }
    }

    for (int y = p1.y; y <= p2.y; ++y) {
        float alpha = (y - p0.y) / (float) v2.y;
        float betha = (y - p1.y) / ((float) v3.y + 1);
        ivec2 p01 = p1 + ivec2(v3.x * betha,v3.y * betha);
        ivec2 p02 = p0 + ivec2(v2.x * alpha,v2.y * alpha);
        if (p01.x > p02.x)std::swap(p01, p02);
        for (int x = p01.x; x <= p02.x; ++x) {
            image.set(x, y, color);
        }
    }
}

const int width = 300;
const int height = 300;
const float halfWidth = width / 2.0;
const float halfHeight = height / 2.0;

int main(int argc, char **argv) {
    TGAImage image(width, height, TGAImage::RGB);
//    Model m;
//    m.loadObj("../african_head.obj");
//    for (int i = 0; i < m.faces.size(); i++) {
//        Face f = m.faces[i];
//        dvec3 v0 = (m.vertices[f.vert.x] + dvec3(1.0, 1.0, 1.0));
//        dvec3 v1 = (m.vertices[f.vert.y] + dvec3(1.0, 1.0, 1.0));
//        dvec3 v2 = (m.vertices[f.vert.z] + dvec3(1.0, 1.0, 1.0));
//        line(ivec2(v0.x * halfWidth, v0.y * halfHeight), ivec2(v1.x * halfWidth, v1.y * halfHeight), image, white);
//        line(ivec2(v0.x * halfWidth, v0.y * halfHeight), ivec2(v2.x * halfWidth, v2.y * halfHeight), image, white);
//        line(ivec2(v1.x * halfWidth, v1.y * halfHeight), ivec2(v2.x * halfWidth, v2.y * halfHeight), image, white);
//    }
    image.set(30,30,red);
    image.set(10,10,red);
    image.set(0,20,red);
    triangle(ivec2(30, 30), ivec2(10, 10), ivec2(0, 20), image, white);
    triangle(ivec2(0, 20), ivec2(30, 30), ivec2(0, 40), image, red);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}

