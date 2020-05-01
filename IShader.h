//
// Created by menegais1 on 30/04/2020.
//

#ifndef TINYRENDERER_ISHADER_H
#define TINYRENDERER_ISHADER_H


#include "Vectors/Vector3.h"
#include "tgaimage.h"
#include "Vectors/Vector2.h"


class IShader {
public:

    virtual dvec3 vertexShader(int faceId, int vertexId) = 0;

    virtual bool fragmentShader(dvec3 barycentric, TGAColor &color) = 0;
};


#endif //TINYRENDERER_ISHADER_H
