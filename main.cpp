#include "tgaimage.h"
#include "Vectors/Vector2.h"
#include "Model.h"
#include "Render.h"
#include "Vectors/Matrix.h"
#include "Camera.h"
#include <iostream>
#include <cmath>


void loadModelAndRender() {
    Model m;
    m.loadObj("../african_head.obj");
    m.loadDiffuse("../grid.tga");
    m.loadNormal("../african_head_nm_tangent.tga");
    m.renderModel();
}

int main(int argc, char **argv) {
    Camera* camera = Render::getInstance().camera;
    camera->lookAt(dvec3(1, 1, 3), dvec3(0, 0, 0), dvec3(0, 1, 0));
    camera->projection(-1.0 / (camera->cameraPos - camera->cameraPointOfInterest).length());
    camera->viewport(Render::width / 8.0, Render::height / 8.0, Render::width * 3.0 / 4.0, Render::height * 3.0 / 4.0);
    loadModelAndRender();
    return 0;
}

