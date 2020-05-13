//
// Created by menegais1 on 26/04/2020.
//

#include "Model.h"
#include "Vectors/Vector2.h"
#include "Render.h"
#include "Shaders.h"
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

void Model::loadDiffuse(std::string filename) {
    diffuseTexture.read_tga_file(filename.c_str());
    diffuseTexture.flip_vertically();
}

void Model::loadNormal(std::string filename) {
    normalTexture.read_tga_file(filename.c_str());
    normalTexture.flip_vertically();
}

void Model::loadSpecular(std::string filename) {
    specularTexture.read_tga_file(filename.c_str());
    specularTexture.flip_vertically();
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


dvec3 lightPosition = dvec3(0, 1, 1);
dvec3 lightDirection = (lightPosition - (dvec3(0, 0, 0))).unit();


void Model::calculateShadowMap() {
    Camera *camera = Render::getInstance().camera;
    camera->lookAt(lightPosition, dvec3(0, 0, 0), dvec3(0, 1, 0));
    camera->projection(-1.0 / (camera->cameraPos - camera->cameraPointOfInterest).length());
    camera->viewport(Render::width / 8.0, Render::height / 8.0, Render::width * 3.0 / 4.0, Render::height * 3.0 / 4.0);
    ShadowMapShader *shader = new ShadowMapShader(this);
    dvec3 verts[3];

    for (int i = 0; i < faces.size(); i++) {
        for (int j = 0; j < 3; ++j) {
            verts[j] = shader->vertexShader(i, j);
        }

        Render::getInstance().triangleBarycentric(verts, shader, Render::getInstance().image,
                                                  Render::getInstance().shadowBuffer);
    }

    Render::getInstance().worldToShadowMap = camera->Viewport * camera->Projection * camera->View;
    delete shader;
}

void Model::renderModel() {
    Camera *camera = Render::getInstance().camera;


    calculateShadowMap();
    Render::getInstance().image.clear();
    camera->lookAt(dvec3(1, 1, 4), dvec3(0, 0, 0), dvec3(0, 1, 0));
    camera->projection(-1.0 / (camera->cameraPos - camera->cameraPointOfInterest).length());
    camera->viewport(Render::width / 8.0, Render::height / 8.0, Render::width * 3.0 / 4.0, Render::height * 3.0 / 4.0);

    GoroudShader *shader = new GoroudShader(this, lightDirection);
    shader->_WorldToShadowMap = Render::getInstance().worldToShadowMap;
    shader->_ShadowMap = Render::getInstance().shadowBuffer;
    dvec3 verts[3];
    for (int i = 0; i < faces.size(); i++) {
        for (int j = 0; j < 3; ++j) {
            verts[j] = shader->vertexShader(i, j);
        }

        Render::getInstance().triangleBarycentric(verts, shader, Render::getInstance().image,
                                                  Render::getInstance().depthBuffer);
    }
    delete shader;
}

dvec3 Model::vertex(int faceId, int vertexId) {
    return vertices[faces[faceId].vert[vertexId]];
}

dvec3 Model::normal(int faceId, int vertexId) {
    return verticesNormals[faces[faceId].normal[vertexId]];
}

dvec3 Model::uv(int faceId, int vertexId) {
    return textureCoordinates[faces[faceId].texture[vertexId]];
}

dvec3 Model::surfaceNormal(int faceId) {
    Face f = faces[faceId];
    dvec3 v0 = vertices[f.vert[0]];
    dvec3 v1 = vertices[f.vert[1]];
    dvec3 v2 = vertices[f.vert[2]];
    return ((v1 - v0).cross(v2 - v0)).unit();
}

dvec3 Model::interpolate(dvec3 barycentric, dvec3 v0, dvec3 v1, dvec3 v2) {
    dvec3 result;
    result.x = v0.x * barycentric[0] + v1.x * barycentric[1] + v2.x * barycentric[2];
    result.y = v0.y * barycentric[0] + v1.y * barycentric[1] + v2.y * barycentric[2];
    result.z = v0.z * barycentric[0] + v1.z * barycentric[1] + v2.z * barycentric[2];
    return result;
}

dvec3 Model::sampleDiffuse(dvec2 uv) {
    TGAColor c = diffuseTexture.get(uv.x * diffuseTexture.get_width(), uv.y * diffuseTexture.get_height());
    return dvec3(c.r, c.g, c.b);
}


dvec3 Model::sampleNormal(dvec2 uv) {
    TGAColor c = normalTexture.get(uv.x * normalTexture.get_width(), uv.y * normalTexture.get_height());
    return dvec3(c.r, c.g, c.b);
}

dvec3 Model::sampleSpecular(dvec2 uv) {
    TGAColor c = specularTexture.get(uv.x * specularTexture.get_width(), uv.y * specularTexture.get_height());
    return dvec3(c.r, c.g, c.b);
}