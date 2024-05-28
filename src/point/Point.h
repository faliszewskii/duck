//
// Created by faliszewskii on 30.04.24.
//

#ifndef PUMAROBOT_POINT_H
#define PUMAROBOT_POINT_H


#include <memory>
#include "glm/vec3.hpp"
#include "../mesh/Mesh.h"
#include "../mesh/EmptyVertex.h"

class Point {
    std::unique_ptr<Mesh<EmptyVertex>> mesh;
public:
    glm::vec3 position{};
    glm::vec4 color{1};

    Point() {
        mesh = std::make_unique<Mesh<EmptyVertex>>(Mesh<EmptyVertex>({EmptyVertex()}, {}, GL_POINTS));
    }

    void render(Shader &shader) {
        shader.setUniform("position", position);
        shader.setUniform("color", color);
        mesh->render();
    }
};


#endif //PUMAROBOT_POINT_H
