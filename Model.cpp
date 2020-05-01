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
    ModelShader *shader = new ModelShader(this);
    dvec3 verts[3];
    for (int i = 0; i < faces.size(); i++) {
        Face f = faces[i];
        for (int j = 0; j < 3; ++j) {
            verts[j] = shader->vertexShader(i, f.vert[j]);
        }

        Render::getInstance().triangleBarycentric(verts, shader);
    }
    delete shader;
}

dvec3 ModelShader::vertexShader(int faceId, int vertexId) {
    Camera* c = Render::getInstance().camera;
    return Render::matrixToVector(c->Viewport * c->Projection * c->View * m->vertices[vertexId]);
}

bool ModelShader::fragmentShader(dvec3 barycentric, TGAColor &color) {
    color = TGAColor(255, 255, 255, 255);
    return false;
}

ModelShader::ModelShader(Model *m) : m(m) {

}
