//
// Created by faliszewskii on 17.05.24.
//

#define STB_PERLIN_IMPLEMENTATION
#include "stb/stb_perlin.h"
#include "Water.h"

void Water::render(Shader &shader) {
    glDisable(GL_CULL_FACE);
    shader.setUniform("material.albedo", glm::vec4(0.15,0.25,0.45,1));
    shader.setUniform("material.shininess", 256.f);
    shader.setUniform("model", frontModelMtx);
    shader.setUniform("normalMap", 0);
    normalTexture.bind(0);
    shader.setUniform("perlinNoise", 1);
    perlinNoise.bind(1);
    shader.setUniform("seaFloorTexture", 2);
    seaFloorTexture.bind(2);
    shader.setUniform("debugNormal", debugNormal);

//    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    frontMesh.render();
    glEnable(GL_CULL_FACE);
}

void Water::update(Shader &compute, float timeS) {
//    waterLogic.update(timeS);
//    normalTexture.update2D(waterLogic.getNormalMap());

    compute.use();
    normalTexture.bindImage(0);
    heightTexture1.bindImage(1);
    heightTexture2.bindImage(2);
    dimTexture.bindImage(3);

    compute.setUniform("currentTexture", currentTexture);
    glDispatchCompute((unsigned int)gridResolution, (unsigned int)gridResolution, 1);

    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    currentTexture = (currentTexture+1)%2;
}

Water::Water(Texture &perlinNoise) : perlinNoise(perlinNoise) {
    debugNormal = false;
    frontModelMtx = glm::rotate(glm::identity<glm::mat4>(), glm::half_pi<float>(), {0, 0, 1});

    auto dampCoeff  = std::vector<float>(gridResolution*gridResolution*4, 0.95);
    for(int y = 0; y < gridResolution; y++) {
        for (int x = 0; x < gridResolution; x++) {
            int id = y * gridResolution + x;
            glm::vec2 dist = glm::vec2{x, y} / (float)gridResolution - 0.5f;
            float fadeOut = sqrt(dist.x*dist.x + dist.y*dist.y);
            dampCoeff[id*4] *= fmin(fmax(0, -pow((fadeOut*2.f),20)+1), 1);
        }
    }

    dimTexture.update2D(dampCoeff.data());
}

void Water::disturb(float x, float y, float strength) {
    int xId = (int) (x * (float) gridResolution);
    int yId = (int) (y * (float) gridResolution);
    if(xId < 0 || xId >= gridResolution || yId < 0 || yId >= gridResolution) return;
    if(currentTexture == 0)
        heightTexture2.updatePixel(xId, yId, &strength);
    else
        heightTexture1.updatePixel(xId, yId, &strength);

}
