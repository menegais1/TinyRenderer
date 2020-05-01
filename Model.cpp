//
// Created by menegais1 on 26/04/2020.
//

#include "Model.h"
#include "Vectors/Vector2.h"
#include "Render.h"
#include <fstream>
#include <string>
#include <iostream>

void Model::loadObj(std::string filename) {
    std::ifstream file(filename, std::ios::in);
    if (file.is_open()) std::cout << "open" << std::endl;
    else std::cout << "error" << std::endl;
    readVertices(file);
    readTextureCoordinates(file);
    readVerticesNormals(file);
    readFaces(file);
}

void Model::readVertices(std::ifstream &file) {
    std::string x, y, z;
    std::string s;
    while (true) {
        file >> s;
        if (s.length() == 0)break;
        if (s.at(0) == '#' || s.at(0) == '\n') {
            std::getline(file, s);
            continue;
        } else if (s != "v") {
            file.seekg(-s.size(), std::ios::cur);
            break;
        }
        file >> x;
        file >> y;
        file >> z;
        vertices.push_back(dvec3(std::stod(x), std::stod(y), std::stod(z)));
    }
}

void Model::readTextureCoordinates(std::ifstream &file) {
    std::string x, y, z;
    std::string s;
    while (true) {
        file >> s;
        if (s.length() == 0)break;
        if (s.at(0) == '#' || s.at(0) == '\n') {
            std::getline(file, s);
            continue;
        } else if (s != "vt") {
            file.seekg(-s.size(), std::ios::cur);
            break;
        }
        file >> x;
        file >> y;
        file >> z;
        textureCoordinates.push_back(dvec3(std::stod(x), std::stod(y), std::stod(z)));
    }
}

void Model::readVerticesNormals(std::ifstream &file) {
    std::string x, y, z;
    std::string s;
    while (true) {
        file >> s;
        if (s.length() == 0)break;
        if (s.at(0) == '#' || s.at(0) == '\n') {
            std::getline(file, s);
            continue;
        } else if (s != "vn") {
            file.seekg(-s.size(), std::ios::cur);
            break;
        }
        file >> x;
        file >> y;
        file >> z;
        verticesNormals.push_back(dvec3(std::stod(x), std::stod(y), std::stod(z)));
    }
}

void Model::readFaces(std::ifstream &file) {
    std::string v0, v1, v2, vt0, vt1, vt2, vn0, vn1, vn2;
    std::string s;
    while (!file.eof()) {
        std::getline(file, s, ' ');
        if (s.length() == 0)break;
        if (s.at(0) == '#' || s.at(0) == '\n') {
            std::getline(file, s);
            continue;
        } else if (s != "f") {
            file.seekg(-s.size(), std::ios::cur);
            break;
        }
        std::getline(file, v0, '/');
        std::getline(file, vt0, '/');
        std::getline(file, vn0, ' ');
        std::getline(file, v1, '/');
        std::getline(file, vt1, '/');
        std::getline(file, vn1, ' ');
        std::getline(file, v2, '/');
        std::getline(file, vt2, '/');
        std::getline(file, vn2, '\n');
        ivec3 vert = ivec3(std::stoi(v0) - 1, std::stoi(v1) - 1, std::stoi(v2) - 1);
        ivec3 texture = ivec3(std::stoi(vt0) - 1, std::stoi(vt1) - 1, std::stoi(vt2) - 1);
        ivec3 normals = ivec3(std::stoi(vn0) - 1, std::stoi(vn1) - 1, std::stoi(vn2) - 1);
        faces.push_back({vert, texture, normals});
    }
}

void Model::renderModel() {
    TGAImage texture;
    texture.read_tga_file("../african_head_diffuse.tga");
    texture.flip_vertically();
    FlatShader *shader = new FlatShader(this, dvec3(0, 0, 0),
                                        (dvec3(0, 0, 0) - dvec3(1, 1, 1)).unit(), dvec3(255, 255, 255));
    dvec3 verts[3];
    for (int i = 0; i < faces.size(); i++) {
        for (int j = 0; j < 3; ++j) {
            verts[j] = shader->vertexShader(i, j);
        }

        Render::getInstance().triangleBarycentric(verts, shader);
    }
    delete shader;
}

dvec3 Model::vertex(int faceId, int vertexId) {
    return vertices[faces[faceId].vert[vertexId]];
}

dvec3 Model::normal(int faceId, int vertexId) {
    return verticesNormals[faces[faceId].vert[vertexId]];
}

dvec3 Model::uv(int faceId, int vertexId) {
    return textureCoordinates[faces[faceId].vert[vertexId]];
}

dvec3 Model::surfaceNormal(int faceId) {
    Face f = faces[faceId];
    dvec3 v0 = vertices[f.vert[0]];
    dvec3 v1 = vertices[f.vert[1]];
    dvec3 v2 = vertices[f.vert[2]];
    return ((v2 - v0).cross(v1 - v0)).unit();
}

dvec3 FlatShader::vertexShader(int faceId, int vertexId) {
    Camera *c = Render::getInstance().camera;

    dvec3 vertex = _Model->vertex(faceId, vertexId);
    dvec3 normal = _Model->surfaceNormal(faceId);
    varyingLightIntensity = std::max(0.0, normal.dot(_PointLightDirection));
    return Render::matrixToVector(c->Viewport * c->Projection * c->View * vertex);
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

