//
// Created by faliszewskii on 25.05.24.
//

#ifndef PUMAROBOT_LINE_H
#define PUMAROBOT_LINE_H

#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "../Shader.h"
#include "../mesh/EmptyVertex.h"
#include "../mesh/Mesh.h"

class Line {
    std::unique_ptr<Mesh<EmptyVertex>> mesh;
public:
    glm::vec3 positionStart{};
    glm::vec3 positionEnd{};
    glm::vec4 color{1};

    Line() {
        mesh = std::make_unique<Mesh<EmptyVertex>>(Mesh<EmptyVertex>({EmptyVertex()}, {}, GL_POINTS));
    }

    void render(Shader &shader) {
        shader.setUniform("positionStart", positionStart);
        shader.setUniform("positionEnd", positionEnd);
        shader.setUniform("color", color);
        mesh->render();
    }
};

#endif //PUMAROBOT_LINE_H
