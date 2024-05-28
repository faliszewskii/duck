//
// Created by faliszewskii on 18.05.24.
//

#include "Sky.h"

void Sky::render(Shader &shader) {
    shader.setUniform("model", modelMtx);
    mesh.render();
}

Sky::Sky() {
    modelMtx = glm::translate(glm::identity<glm::mat4>(), {0, 0, 0});
    modelMtx = glm::rotate(modelMtx, -glm::half_pi<float>(), {0, 1, 0});
    modelMtx = glm::scale(modelMtx, {1, 2, 2});
}
