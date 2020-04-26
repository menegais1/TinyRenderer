#include "tgaimage.h"
#include "Vectors/Vector2.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

//Bresenham line algorithm implementation
//Extra explanation: https://www.youtube.com/watch?v=IDFB5CDpLDE
void line(ivec2 p0, ivec2 p1, TGAImage &image, TGAColor color) {
    int height = abs(p1.y - p0.y);
    int width = abs(p1.x - p0.y);
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

int main(int argc, char **argv) {
    TGAImage image(100, 100, TGAImage::RGB);
    line(ivec2(13, 20), ivec2(80, 40), image, white);
    line(ivec2(20, 13), ivec2(40, 80), image, red);
    line(ivec2(80, 40), ivec2(13, 20), image, red);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}

