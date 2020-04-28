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

void Model::renderWireframe(TGAImage &image, const TGAColor &color, int scaleX, int scaleY) {
    for (int i = 0; i < faces.size(); i++) {
        Face f = faces[i];
        dvec3 v0 = (vertices[f.vert.x] + dvec3(1.0, 1.0, 1.0));
        dvec3 v1 = (vertices[f.vert.y] + dvec3(1.0, 1.0, 1.0));
        dvec3 v2 = (vertices[f.vert.z] + dvec3(1.0, 1.0, 1.0));
        line(ivec2(v0.x * scaleX, v0.y * scaleY), ivec2(v1.x * scaleX, v1.y * scaleY), image, color);
        line(ivec2(v0.x * scaleX, v0.y * scaleY), ivec2(v2.x * scaleX, v2.y * scaleY), image, color);
        line(ivec2(v1.x * scaleX, v1.y * scaleY), ivec2(v2.x * scaleX, v2.y * scaleY), image, color);
    }
}

void Model::renderModel(TGAImage &image, int scaleX, int scaleY, dvec3 lightDirection) {
    float *zBuffer = new float[width * height];
    for (int j = 0; j < width * height; ++j) {
        //Some strange bug happened
        zBuffer[j] =  1.175494e-38;
    }

    TGAImage texture;
    texture.read_tga_file("../african_head_diffuse.tga");
    texture.flip_vertically();
    for (int i = 0; i < faces.size(); i++) {
        Face f = faces[i];
        dvec3 v0 = (vertices[f.vert.x] + dvec3(1.0, 1.0, 1.0));
        dvec3 v1 = (vertices[f.vert.y] + dvec3(1.0, 1.0, 1.0));
        dvec3 v2 = (vertices[f.vert.z] + dvec3(1.0, 1.0, 1.0));
        v0 = dvec3((v0.x * scaleX), (v0.y * scaleY), v0.z);
        v1 = dvec3((v1.x * scaleX), (v1.y * scaleY), v1.z);
        v2 = dvec3((v2.x * scaleX), (v2.y * scaleY), v2.z);
        dvec3 uv0 = textureCoordinates[f.texture.x];
        dvec3 uv1 = textureCoordinates[f.texture.y];
        dvec3 uv2 = textureCoordinates[f.texture.z];
        dvec3 normal = (v2 - v0).cross(v1 - v0);
        normal = normal.unit();
        float lightIntensity = normal.dot(lightDirection);
        if (lightIntensity >= 0) {
            triangleBarycentric(v0, v1, v2, zBuffer, image,
                                texture, dvec2(uv0.x, uv0.y), dvec2(uv1.x, uv1.y), dvec2(uv2.x, uv2.y), lightIntensity);
        }
    }
}

