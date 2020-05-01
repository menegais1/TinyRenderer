//
// Created by menegais1 on 26/04/2020.
//

#ifndef TINYRENDERER_MODEL_H
#define TINYRENDERER_MODEL_H

#include <vector>
#include <string>
#include <fstream>
#include "Vectors/Vector3.h"
#include "tgaimage.h"
#include "IShader.h"


typedef struct face {
    ivec3 vert;
    ivec3 texture;
    ivec3 normal;
} Face;


class ModelShader;

class Model {
public:
    std::vector<Face> faces;
    std::vector<dvec3> vertices;
    std::vector<dvec3> textureCoordinates;
    std::vector<dvec3> verticesNormals;

    void loadObj(std::string filename);

    void renderWireframe(TGAImage &image, const TGAColor &color, int scaleX, int scaleY);

    void renderModel();

private:
    void readVertices(std::ifstream &file);

    void readTextureCoordinates(std::ifstream &file);

    void readVerticesNormals(std::ifstream &file);

    void readFaces(std::ifstream &file);

};


class ModelShader : public IShader {
public:

    Model *m;

    ModelShader(Model* m);

    dvec3 vertexShader(int faceId, int vertexId) override;

    bool fragmentShader(dvec3 barycentric, TGAColor &color) override;
};

#endif //TINYRENDERER_MODEL_H
