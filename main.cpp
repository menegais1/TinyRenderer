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
    m.renderModel();
}

int main(int argc, char **argv) {
    Camera* camera = Render::getInstance().camera;
    camera->lookAt(dvec3(0, 0, 3), dvec3(0, 0, 0), dvec3(0, 1, 0));
    camera->projection(-1.0 / (camera->cameraPos - camera->cameraPointOfInterest).length());
    camera->viewport(Render::width / 8, Render::width / 8, Render::width * 3 / 4, Render::width * 3 / 4);
    loadModelAndRender();
    return 0;
}

