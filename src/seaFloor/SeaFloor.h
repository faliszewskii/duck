//
// Created by faliszewskii on 17.05.24.
//

#ifndef PUMAROBOT_SEAFLOOR_H
#define PUMAROBOT_SEAFLOOR_H


#include "../quad/Quad.h"
#include "../texture/Texture.h"

class SeaFloor {
    glm::mat4 modelMtx;
    Quad mesh;
    Texture texture{"../res/textures/sea_floor.png", GL_MIRRORED_REPEAT};

public:
    SeaFloor() {
        deepness = 6.f;
        modelMtx = glm::translate(glm::identity<glm::mat4>(), {0, 0, 1});
        modelMtx = glm::rotate(modelMtx, -glm::half_pi<float>(), {0, 1, 0});
        modelMtx = glm::scale(modelMtx, {1, 2, 2});
    }

    void render(Shader &shader);

    float deepness;
};


#endif //PUMAROBOT_SEAFLOOR_H
