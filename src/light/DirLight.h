//
// Created by faliszewskii on 17.05.24.
//

#ifndef PUMAROBOT_DIRLIGHT_H
#define PUMAROBOT_DIRLIGHT_H

#include "../Shader.h"

struct DirLight {
    glm::vec3 direction = glm::normalize(glm::vec3(0.2, -1, 0.2));
    glm::vec3 color = glm::vec3(255/255.f, 214/255.f, 170/255.f);
    float strength = 1.f;

    void setup(Shader& shader) {
        shader.setUniform("dirLight.direction", direction);
        shader.setUniform("dirLight.color", color);
        shader.setUniform("dirLight.strength", strength);
    }
};

#endif //PUMAROBOT_DIRLIGHT_H
