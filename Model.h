//
// Created by menegais1 on 26/04/2020.
//

#ifndef TINYRENDERER_MODEL_H
#define TINYRENDERER_MODEL_H

#include <vector>
#include <string>
#include <fstream>
#include "Vectors/Vector3.h"

typedef struct face{
    ivec3 vert;
    ivec3 texture;
    ivec3 normal;
}Face;

class Model {
public:
    std::vector<Face> faces;
    std::vector<dvec3> vertices;
    std::vector<dvec3> textureCoordinates;
    std::vector<dvec3> verticesNormals;


    void loadObj(std::string filename);

private:
    void readVertices(std::ifstream& file);
    void readTextureCoordinates(std::ifstream& file);
    void readVerticesNormals(std::ifstream& file);
    void readFaces(std::ifstream& file);
};


#endif //TINYRENDERER_MODEL_H
