//
// Created by faliszewskii on 18.04.24.
//

#include <random>
#include "Scene.h"


Scene::Scene(AppContext &appContext) :
        pointShader(Shader::createTraditionalShader("../res/shaders/point/point.vert", "../res/shaders/point/point.frag")),
        lineShader(Shader::createTraditionalShader("../res/shaders/line/line.vert", "../res/shaders/line/line.geom", "../res/shaders/line/line.frag")),
        bSplineShader(Shader::createTraditionalShader("../res/shaders/bspline/bspline.vert", "../res/shaders/bspline/bspline.tesc", "../res/shaders/bspline/bspline.tese", "../res/shaders/bspline/bspline.frag")),
        waterShader(Shader::createTraditionalShader("../res/shaders/water/water.vert", "../res/shaders/water/water.frag")),
        phongShader(Shader::createTraditionalShader("../res/shaders/phong/phong.vert", "../res/shaders/phong/phong.frag")),
        infiniteShader(Shader::createTraditionalShader("../res/shaders/infinite/infinite.vert", "../res/shaders/infinite/infinite.frag")),
        seaShader(Shader::createTraditionalShader("../res/shaders/sea/sea.vert", "../res/shaders/sea/sea.frag")),
        skyShader(Shader::createTraditionalShader("../res/shaders/sky/sky.vert", "../res/shaders/sky/sky.frag")),
        waterHeightCompute(Shader::createComputeShader("../res/compute/water/water.comp")),
        appContext(appContext)
    {}

void Scene::update() {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0,1);

    auto timeS = float(glfwGetTime());
    appContext.water->update(waterHeightCompute, timeS);
    appContext.duck->update(timeS);
    if(!appContext.duck->animation->running) appContext.duck->animation->start();

    static float lastTime = (float)glfwGetTime();
    static float dropletsToDrop = 0;
    dropletsToDrop += (timeS - lastTime) * appContext.dropletsPerSec;
    lastTime = timeS;

    while(dropletsToDrop >= 1) {
        appContext.water->disturb(dist(rng), dist(rng), .05);
        dropletsToDrop -= 1;
    }

    static float timeFromDuck = timeS+0.01;
    static bool frame1 = false;
    if(timeS - timeFromDuck > .1/*appContext.duck->animation->getCurrentFrame() == 0*/) {
//        if(!frame1) {
            timeFromDuck = timeS;
            glm::vec3 duckPosOffset = glm::vec3(-0.04*appContext.duck->animation->getScaleFrame().x, 0, 0);
            duckPosOffset = glm::rotate(glm::mat4(1.0f), appContext.duck->getRotation(), glm::vec3(0,1,0)) * glm::vec4(duckPosOffset, 1);
            glm::vec3 duckPos = appContext.duck->getPosition();// + duckPosOffset;
            appContext.water->disturb(duckPos.z+0.5, -duckPos.x+0.5, 0.05);
//        }
//        frame1 = true;
//    } else {
//        frame1 = false;
    }
}

void Scene::render() {
    appContext.frameBufferManager->bind();

    auto time = float(glfwGetTime());

    skyShader.use();
    skyShader.setUniform("viewPos", appContext.camera->getViewPosition());
    skyShader.setUniform("view", appContext.camera->getViewMatrix());
    skyShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    appContext.preethamSkyModel->set_render_uniforms(skyShader);
    appContext.sky->render(skyShader);

    infiniteShader.use();
    infiniteShader.setUniform("viewPos", appContext.camera->getViewPosition());
    infiniteShader.setUniform("view", appContext.camera->getViewMatrix());
    infiniteShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    infiniteShader.setUniform("far", appContext.camera->getFarPlane());
    infiniteShader.setUniform("near", appContext.camera->getNearPlane());
    appContext.sunLight->setup(infiniteShader);
    appContext.seaFloor->render(infiniteShader);

    waterShader.use();
    waterShader.setUniform("viewPos", appContext.camera->getViewPosition());
    waterShader.setUniform("view", appContext.camera->getViewMatrix());
    waterShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    waterShader.setUniform("time", time);
    waterShader.setUniform("deepness", appContext.seaFloor->deepness);
    appContext.preethamSkyModel->set_render_uniforms(waterShader);
    appContext.preethamSkyModel->set_render_uniforms(waterShader);
    appContext.perlin->setup(waterShader);
    appContext.sunLight->setup(waterShader);
    appContext.water->render(waterShader);

    seaShader.use();
    seaShader.setUniform("viewPos", appContext.camera->getViewPosition());
    seaShader.setUniform("view", appContext.camera->getViewMatrix());
    seaShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    seaShader.setUniform("far", appContext.camera->getFarPlane());
    seaShader.setUniform("near", appContext.camera->getNearPlane());
    seaShader.setUniform("time", time);
    seaShader.setUniform("deepness", appContext.seaFloor->deepness);
    appContext.preethamSkyModel->set_render_uniforms(seaShader);
    appContext.perlin->setup(seaShader);
    appContext.sunLight->setup(seaShader);
    appContext.sea->render(seaShader);

    phongShader.use();
    phongShader.setUniform("viewPos", appContext.camera->getViewPosition());
    phongShader.setUniform("view", appContext.camera->getViewMatrix());
    phongShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    appContext.sunLight->setup(phongShader);
    appContext.duck->render(phongShader);

    pointShader.use();
    pointShader.setUniform("view", appContext.camera->getViewMatrix());
    pointShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    if(appContext.duck->debugDuckTruck)
        appContext.duck->renderDeBoorPoints(pointShader);

    lineShader.use();
    lineShader.setUniform("view", appContext.camera->getViewMatrix());
    lineShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    if(appContext.duck->debugDuckTruck)
        appContext.duck->renderTrackRadii(lineShader);

    bSplineShader.use();
    bSplineShader.setUniform("view", appContext.camera->getViewMatrix());
    bSplineShader.setUniform("projection", appContext.camera->getProjectionMatrix());
    if(appContext.duck->debugDuckTruck)
        appContext.duck->renderTrackCurve();

    appContext.frameBufferManager->unbind();
}
