//
// Created by faliszewskii on 21.05.24.
//

#include "Animation.h"

#include <utility>
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_float.hpp"

Animation::Animation(std::vector<KeyFrame> &keyFrames, bool loop) : loop(loop) {
    float cumulativeTime = 0;
    for(auto &keyFrame : keyFrames) {
        cumulativeTime += keyFrame.duration;
        keyFrameStartTimes.emplace(cumulativeTime, keyFrame);
    }
    animationDuration = cumulativeTime;
    this->keyFrames = std::move(keyFrames);

    translation = glm::vec3(0.0f);
    rotation = glm::identity<glm::quat>();
    scale = glm::vec3(1.0f);

    running = false;
}

void Animation::update(float time) {
    if(!running) return;
    auto transformation = glm::identity<glm::mat4>();

    float animationTime = time - startTime;
    if(animationTime < 0) {
        setTransformation(glm::vec3(0.0f), glm::identity<glm::quat>(), glm::vec3(1.0f));
        currentFrame = 0;
        return;
    }
    if(animationTime > animationDuration && !loop) {
        auto last = keyFrames[keyFrames.size()-1];
        setTransformation(last.translation, last.rotation, last.scale);
        currentFrame = keyFrames.size()-1;
        return;
    } else if(loop) {
        animationTime = std::fmod(animationTime, animationDuration);
    }

    auto currentFrameIt = keyFrameStartTimes.upper_bound(animationTime);
    if(currentFrameIt == keyFrameStartTimes.end()) {
        setTransformation(glm::vec3(0.0f), glm::identity<glm::quat>(), glm::vec3(1.0f));
        currentFrame = 0;
        return;
    }
    currentFrame = std::distance(keyFrameStartTimes.begin(), currentFrameIt);

    glm::vec3 previousTranslation;
    glm::quat previousRotation;
    glm::vec3 previousScale;
    if(currentFrameIt == keyFrameStartTimes.begin()) {
        previousTranslation = glm::vec3();
        previousRotation = glm::identity<glm::quat>();
        previousScale = glm::vec3(1.f);
    } else {
        auto previousFrameIt = std::prev(currentFrameIt);
        previousTranslation = previousFrameIt->second.translation;
        previousRotation = previousFrameIt->second.rotation;
        previousScale = previousFrameIt->second.scale;
    }

    float frameTime = animationTime - (currentFrameIt->first - currentFrameIt->second.duration);
    float progress = frameTime / currentFrameIt->second.duration;


    glm::vec3 currentTranslation = currentFrameIt->second.translation;
    glm::quat currentRotation = currentFrameIt->second.rotation;
    glm::vec3 currentScale = currentFrameIt->second.scale;

    glm::vec3 iTranslate;
    glm::vec3 iScale;
    switch(currentFrameIt->second.interpolation) {
        case KeyFrame::LINEAR:
            iTranslate = currentTranslation * progress + previousTranslation * (1 - progress);
            iScale = currentScale * progress + previousScale * (1 - progress);
            break;
    }

    setTransformation(iTranslate, currentRotation, iScale);
}

void Animation::start() {
    startTime = (float)glfwGetTime();
    running = true;
}

void Animation::stop() {
    running = false;
}

glm::vec3 Animation::getTranslationFrame() {
    return translation;
}

glm::quat Animation::getRotationFrame() {
    return rotation;
}

glm::vec3 Animation::getScaleFrame() {
    return scale;
}

void Animation::setTransformation(glm::vec3 translation, glm::quat rotation, glm::vec3 scale) {

    this->translation = translation;
    this->rotation = rotation;
    this->scale = scale;
}

int Animation::getCurrentFrame() {
    return currentFrame;
}
