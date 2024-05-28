//
// Created by faliszewskii on 18.05.24.
//

#ifndef PUMAROBOT_SKY_H
#define PUMAROBOT_SKY_H


#include "../Shader.h"
#include "../quad/Quad.h"
#include "PreethamSkyModel.h"

class Sky {
    glm::mat4 modelMtx;
    Quad mesh;

public:

    explicit Sky();

    void render(Shader &shader);
};


#endif //PUMAROBOT_SKY_H
