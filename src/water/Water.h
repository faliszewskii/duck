//
// Created by faliszewskii on 17.05.24.
//

#ifndef PUMAROBOT_WATER_H
#define PUMAROBOT_WATER_H


#include "../quad/Quad.h"
#include "../texture/Texture.h"

class Water {
public:
    int gridResolution = 512;
    bool debugNormal;
private:
    glm::mat4 frontModelMtx;
    Quad frontMesh;

    Texture normalTexture{gridResolution, gridResolution, 4,GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_TEXTURE_2D};
    Texture heightTexture1{gridResolution, gridResolution, 4, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_TEXTURE_2D};
    Texture heightTexture2{gridResolution, gridResolution,4, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_TEXTURE_2D};
    Texture dimTexture{gridResolution, gridResolution,4, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_TEXTURE_2D};
    int currentTexture = 0;

    Texture &perlinNoise;
    Texture seaFloorTexture{"../res/textures/sea_floor.png", GL_MIRRORED_REPEAT};

public:

    explicit Water(Texture &perlinNoise);

    void update(Shader &compute, float timeS);

    void disturb(float x, float y, float strength);

    void render(Shader &shader);
};


#endif //PUMAROBOT_WATER_H
