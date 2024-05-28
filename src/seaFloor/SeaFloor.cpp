//
// Created by faliszewskii on 17.05.24.
//

#include "SeaFloor.h"

void SeaFloor::render(Shader &shader) {
    shader.setUniform("model", modelMtx);
    shader.setUniform("material.albedo", glm::vec4(0.7,0.7,0.5,1));
    shader.setUniform("material.shininess", 256.f);
    shader.setUniform("material.hasTexture", true);
    shader.setUniform("material.texture", 0);
    shader.setUniform("yLevel", -deepness);
    texture.bind(0);
    mesh.render();
}
