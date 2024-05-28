//
// Created by faliszewskii on 25.05.24.
//

#ifndef PUMAROBOT_BSPLINE_H
#define PUMAROBOT_BSPLINE_H

#include <memory>
#include "../mesh/Mesh.h"
#include "../mesh/PositionVertex.h"
#include "../Shader.h"

class BSpline {
    std::unique_ptr<Mesh<PositionVertex>> mesh;

public:
    BSpline() {
        mesh = std::make_unique<Mesh<PositionVertex>>(Mesh<PositionVertex>({}, {}, GL_PATCHES));
    }

    void update(std::vector<PositionVertex> controlPoints) {
        mesh->update(std::move(controlPoints));
    }

    void render() {
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        mesh->render();
    }

};

#endif //PUMAROBOT_BSPLINE_H
