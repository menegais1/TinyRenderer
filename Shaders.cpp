//
// Created by menegais1 on 11/05/2020.
//

#include "Shaders.h"
#include "Render.h"
#include <cmath>

dvec3 FlatShader::vertexShader(int faceId, int vertexId) {
    Camera *c = Render::getInstance().camera;

    dvec3 vertex = _Model->vertex(faceId, vertexId);
    dvec3 normal = _Model->surfaceNormal(faceId);
    varyingLightIntensity = std::max(0.0, normal.dot(_PointLightDirection));
    return Render::ClipSpaceToNDC(c->Viewport * c->Projection * c->View * vertex.toVector4(1));
}

bool FlatShader::fragmentShader(dvec3 barycentric, TGAColor &color) {
    dvec3 c = _GlobalIluminationColor * 0.3f + _PointLightColor * varyingLightIntensity;
    color = TGAColor(std::min(c.x, 255.0), std::min(c.y, 255.0), std::min(c.z, 255.0), 1);
    return false;
}

FlatShader::FlatShader(Model *_Model, const dvec3 &globalIluminationColor, const dvec3 &pointLightDirection,
                       const dvec3 &pointLightColor) : _Model(_Model),
                                                       _GlobalIluminationColor(globalIluminationColor),
                                                       _PointLightDirection(pointLightDirection),
                                                       _PointLightColor(pointLightColor) {}

dvec3 GoroudShader::vertexShader(int faceId, int vertexId) {
    Camera *c = Render::getInstance().camera;

    dvec3 vertex = _Model->vertex(faceId, vertexId);
    dvec3 normal = _Model->normal(faceId, vertexId);
    varyingUv[vertexId] = _Model->uv(faceId, vertexId);
    varyingVertex[vertexId] = _Model->vertex(faceId, vertexId);
    varyingNormal[vertexId] = _Model->normal(faceId, vertexId);
    varyingLightIntensity[vertexId] = std::max(0.0, normal.dot(_DirectionalLightDirection));
    varyingNDCVertex[vertexId] = Render::ClipSpaceToNDC(c->Viewport * c->Projection * c->View * vertex.toVector4(1));
    varyingClipSpaceToWorld = (c->Viewport * c->Projection * c->View).invert();
    return Render::ClipSpaceToNDC(c->Viewport * c->Projection * c->View * vertex.toVector4(1));
}

bool GoroudShader::fragmentShader(dvec3 barycentric, TGAColor &color) {
    Camera *c = Render::getInstance().camera;
    float lightIntensity = barycentric.dot(varyingLightIntensity);

    dvec3 uv = _Model->interpolate(barycentric, varyingUv[0], varyingUv[1], varyingUv[2]);
    dvec3 normal = _Model->interpolate(barycentric, varyingNormal[0], varyingNormal[1], varyingNormal[2]).unit();
    dvec3 worldPos = _Model->interpolate(barycentric, varyingVertex[0], varyingVertex[1], varyingVertex[2]);

    dvec3 shadowPos = _Model->interpolate(barycentric, varyingNDCVertex[0], varyingNDCVertex[1],
                                          varyingNDCVertex[2]);
    shadowPos = Render::ClipSpaceToNDC(_WorldToShadowMap * varyingClipSpaceToWorld * shadowPos.toVector4(1));
    int idx = shadowPos.y * Render::width + shadowPos.x;
    float shadowAmount = 0;
    if (_ShadowMap[idx] > shadowPos.z +43.34) {
        shadowAmount += 1;
    } else {
        shadowAmount = 0;
    }
    Matrix<double> TBN = CalculateTBN(uv, normal);

    dvec3 tmp = _Model->sampleNormal(dvec2(uv.x, uv.y));
    dvec3 disturbedNormal;
    for (int i = 0; i < 3; i++)
        disturbedNormal[i] = (float) tmp[i] / 255.f * 2.f - 1.f;

    auto RN = TBN * disturbedNormal;
    dvec3 resultNormal = dvec3(RN[0][0], RN[1][0], RN[2][0]).unit();

    dvec3 texColor = _Model->sampleDiffuse(dvec2(uv.x, uv.y));
    dvec3 specSample = _Model->sampleSpecular(dvec2(uv.x, uv.y));
    dvec3 perfectReflectionDirection =
            (resultNormal * (2.0 * resultNormal.dot(_DirectionalLightDirection)) - _DirectionalLightDirection).unit();
    double specIntensity = 0;
    if (specSample.z != 0)
        specIntensity = std::pow(
                std::max(0.0, perfectReflectionDirection.dot((c->cameraPos - worldPos).unit())),
                specSample.z / 1.0);

    float diffuseIntensity = std::max(0.0, resultNormal.dot(_DirectionalLightDirection));
    if (diffuseIntensity == 0) specIntensity = 0;
      dvec3 finalLight = texColor * shadowAmount * (1.2 * diffuseIntensity /*+ .6f * specIntensity*/) + dvec3(5, 5, 5);
//    dvec3 finalLight = dvec3(_ShadowMap[idx],
//                             _ShadowMap[idx],
//                             _ShadowMap[idx]);
//    dvec3 finalLight = dvec3(shadowPos.z ,
//                             shadowPos.z ,
//                             shadowPos.z );
    color = TGAColor(std::min(finalLight.x, 255.0), std::min(finalLight.y, 255.0), std::min(finalLight.z, 255.0), 1);

    return false;
}

//Code explanation: http://www.thetenthplanet.de/archives/1180
Matrix<double> GoroudShader::CalculateTBN(const dvec3 &uv, const dvec3 &normal) const {
    Matrix<double> A(3, 3);
    dvec3 deltaV1 = (varyingVertex[1] - varyingVertex[0]);
    dvec3 deltaV2 = (varyingVertex[2] - varyingVertex[0]);
    dvec3 cross = deltaV1.cross(deltaV2).unit();
    A.setRow(deltaV1, 0);
    A.setRow(deltaV2, 1);
    A.setRow(normal, 2);
    auto AI = A.invert();

    dvec3 deltaU = dvec3(varyingUv[1].x - varyingUv[0].x, varyingUv[2].x - varyingUv[0].x, 0);
    dvec3 deltaV = dvec3(varyingUv[1].y - varyingUv[0].y, varyingUv[2].y - varyingUv[0].y, 0);

    auto Tm = AI * deltaU;
    auto Bm = AI * deltaV;
    dvec3 T = dvec3(Tm[0][0], Tm[1][0], Tm[2][0]).unit();
    dvec3 B = dvec3(Bm[0][0], Bm[1][0], Bm[2][0]).unit();

    A.setCol(T, 0);
    A.setCol(B, 1);
    A.setCol(normal, 2);

    return A;
}

GoroudShader::GoroudShader(Model *_Model, const dvec3 &_DirectionalLightDirection) : _Model(_Model),
                                                                                     _DirectionalLightDirection(
                                                                                             _DirectionalLightDirection) {}

dvec3 ShadowMapShader::vertexShader(int faceId, int vertexId) {
    Camera *c = Render::getInstance().camera;
    dvec3 vertex = _Model->vertex(faceId, vertexId);
    //The original tutorial says that this transformation (c->Projection * c->View * vertex.toVector4(1)) maps to a unit cube [-1,1]x[-1,1]x[-1,1], this doesn't seem
    //to happen, as the z coordinate gets mapped to [0,-d], 'd' defined by (camera->cameraPos - camera->cameraPointOfInterest).length(),
    //This causes some problems when creating the shadow map, so need to look more into it.
    dvec3 ndcVertex = Render::ClipSpaceToNDC(c->Viewport * c->Projection * c->View * vertex.toVector4(1));
    varyingVertex[vertexId] = ndcVertex;
    return ndcVertex;
}

bool ShadowMapShader::fragmentShader(dvec3 barycentric, TGAColor &color) {
    dvec3 vertex = _Model->interpolate(barycentric, varyingVertex[0], varyingVertex[1], varyingVertex[2]);
    float intensity = vertex.z / Render::depth;
    color = TGAColor(255.0 * intensity);
    return false;
}

ShadowMapShader::ShadowMapShader(Model *model) : _Model(model) {}