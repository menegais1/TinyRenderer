//
// Created by menegais1 on 11/05/2020.
//

#ifndef TINYRENDERER_SHADERS_H
#define TINYRENDERER_SHADERS_H

#include "IShader.h"
#include "Model.h"
#include "Vectors/Vector3.h"
#include "Vectors/Matrix.h"

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
    dvec3 _DirectionalLightDirection;
    float *_ShadowMap;
    Matrix<double> _WorldToShadowMap;

    dvec3 varyingLightIntensity;
    dvec3 varyingUv[3];
    dvec3 varyingNormal[3];
    dvec3 varyingVertex[3];


    GoroudShader(Model *_Model, const dvec3 &_DirectionalLightDirection);

    dvec3 vertexShader(int faceId, int vertexId) override;

    bool fragmentShader(dvec3 barycentric, TGAColor &color) override;

    Matrix<double> CalculateTBN(const dvec3 &uv, const dvec3 &normal) const;
};

class ShadowMapShader : public IShader {
public:

    Model *_Model;
    dvec3 varyingVertex[3];

    ShadowMapShader(Model *model);

    dvec3 vertexShader(int faceId, int vertexId) override;

    bool fragmentShader(dvec3 barycentric, TGAColor &color) override;
};

#include "Model.h"

#endif //TINYRENDERER_SHADERS_H
