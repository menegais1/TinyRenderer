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


class FlatShader;

class Model {
public:
    std::vector<Face> faces;
    std::vector<dvec3> vertices;
    std::vector<dvec3> textureCoordinates;
    std::vector<dvec3> verticesNormals;

    void loadObj(std::string filename);

    void renderModel();

    dvec3 vertex(int faceId, int vertexd);

    dvec3 normal(int faceId, int vertexId);

    dvec3 uv(int faceId, int vertexId);

    dvec3 surfaceNormal(int faceId);

private:
    void readVertices(std::ifstream &file);

    void readTextureCoordinates(std::ifstream &file);

    void readVerticesNormals(std::ifstream &file);

    void readFaces(std::ifstream &file);

};


class FlatShader : public IShader {
public:

    Model *_Model;
    dvec3 _GlobalIluminationColor;
    dvec3 _PointLightDirection;
    dvec3 _PointLightColor;

    float varyingLightIntensity;


    FlatShader(Model *_Model, const dvec3 &globalIluminationColor, const dvec3 &pointLightDirection,
               const dvec3 &pointLightColor);

    dvec3 vertexShader(int faceId, int vertexId) override;

    bool fragmentShader(dvec3 barycentric, TGAColor &color) override;
};

class GoroudShader : public IShader {
public:

    Model *_Model;
    dvec3 _GlobalIluminationColor;
    dvec3 _PointLightDirection;
    dvec3 _PointLightColor;
    TGAImage _DiffuseTexture;

    dvec3 varyingLightIntensity;
    dvec3 varyingUv[3];


    GoroudShader(Model *_Model, const dvec3 &globalIluminationColor, const dvec3 &pointLightDirection,
                 const dvec3 &pointLightColor);

    dvec3 vertexShader(int faceId, int vertexId) override;

    bool fragmentShader(dvec3 barycentric, TGAColor &color) override;
};

#endif //TINYRENDERER_MODEL_H
