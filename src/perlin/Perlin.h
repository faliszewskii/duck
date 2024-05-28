//
// Created by faliszewskii on 18.05.24.
//

#ifndef PUMAROBOT_PERLIN_H
#define PUMAROBOT_PERLIN_H

#include "../Shader.h"

struct Perlin {
    float amplitude = 0.1;
    float frequency = 8;
    float length = 3.f;
    float speed = 0.01f;

    void setup(Shader& shader) {
        shader.setUniform("perlin.frequency", frequency);
        shader.setUniform("perlin.amplitude", amplitude);
        shader.setUniform("perlin.length", length);
        shader.setUniform("perlin.speed", speed);
    }
};

#endif //PUMAROBOT_PERLIN_H
