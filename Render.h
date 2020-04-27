//
// Created by menegais1 on 27/04/2020.
//

#ifndef TINYRENDERER_RENDER_H
#define TINYRENDERER_RENDER_H


#include "Vectors/Vector2.h"
#include "tgaimage.h"

const int width = 600;
const int height = 600;
const float halfWidth = width / 2.0;
const float halfHeight = height / 2.0;

//Line sweep triangle generation algorithm
void triangle(ivec2 p0, ivec2 p1, ivec2 p2, TGAImage &image, TGAColor color);

//Bresenham line algorithm implementation
//Extra explanation: https://www.youtube.com/watch?v=IDFB5CDpLDE
void line(ivec2 p0, ivec2 p1, TGAImage &image, TGAColor color);

#endif //TINYRENDERER_RENDER_H
