//
// Created by faliszewskii on 17.05.24.
//

#ifndef PUMAROBOT_QUAD_H
#define PUMAROBOT_QUAD_H


#include "../mesh/PosNorTexVertex.h"
#include "../mesh/Mesh.h"

class Quad {
    std::unique_ptr<Mesh<PosNorTexVertex>> mesh;

public:
    Quad() {
        std::vector<PosNorTexVertex> vertices = {
                // Front face
                {glm::vec3(0, -0.5f, -0.5f), glm::vec3(+1.0f, 0.0f, 0.0f), glm::vec2(0, 0)},
                {glm::vec3(0, -0.5f, +0.5f), glm::vec3(+1.0f, 0.0f, 0.0f), glm::vec2(1,0)},
                {glm::vec3(0, +0.5f, +0.5f), glm::vec3(+1.0f, 0.0f, 0.0f), glm::vec2(1, 1)},
                {glm::vec3(0, +0.5f, -0.5f), glm::vec3(+1.0f, 0.0f, 0.0f), glm::vec2(0, 1)},
        };
        std::vector<unsigned int> indices = {
                // Back face
                0, 2, 1,
                0, 3, 2,
        };
        mesh = std::make_unique<Mesh<PosNorTexVertex>>(vertices, indices);
    }

    void render() {
        mesh->render();
    }
};


#endif //PUMAROBOT_QUAD_H
