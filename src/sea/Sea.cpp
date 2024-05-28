//
// Created by faliszewskii on 17.05.24.
//

#include "Sea.h"

void Sea::render(Shader &shader) {
    glDisable(GL_CULL_FACE);
    shader.setUniform("model", modelMtx);
    shader.setUniform("material.albedo", glm::vec4(0.15,0.25,0.45, 1.f));
    shader.setUniform("material.shininess", 256.f);
    shader.setUniform("material.hasTexture", false);
    shader.setUniform("perlinNoise", 3);
    perlinNoise.bind(3);
    shader.setUniform("seaFloorTexture", 4);
    seaFloorTexture.bind(4);
    shader.setUniform("yLevel", 0.f);
    mesh.render();
    glEnable(GL_CULL_FACE);
}

Sea::Sea(Texture &perlinNoise) : perlinNoise(perlinNoise) {
    modelMtx = glm::translate(glm::identity<glm::mat4>(), {0, 0, 1});
    modelMtx = glm::rotate(modelMtx, -glm::half_pi<float>(), {0, 1, 0});
    modelMtx = glm::scale(modelMtx, {1, 2, 2});

}
