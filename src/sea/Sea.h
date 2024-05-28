//
// Created by faliszewskii on 17.05.24.
//

#ifndef PUMAROBOT_SEA_H
#define PUMAROBOT_SEA_H


#include "../quad/Quad.h"
#include "glm/fwd.hpp"
#include "../texture/Texture.h"

class Sea {
    glm::mat4 modelMtx;
    Quad mesh;

    Texture &perlinNoise;
    Texture seaFloorTexture{"../res/textures/sea_floor.png", GL_MIRRORED_REPEAT};
public:
    explicit Sea(Texture &perlinNoise);

    void render(Shader &shader);
};


#endif //PUMAROBOT_SEA_H
