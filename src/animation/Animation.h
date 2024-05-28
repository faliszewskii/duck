//
// Created by faliszewskii on 21.05.24.
//

#ifndef PUMAROBOT_ANIMATION_H
#define PUMAROBOT_ANIMATION_H


#include <vector>
#include <map>
#include "glm/ext/matrix_float4x4.hpp"
#include "KeyFrame.h"

class Animation {
    std::vector<KeyFrame> keyFrames;
    std::map<float, KeyFrame&> keyFrameStartTimes;

    bool loop;
    float startTime = 0;
    float animationDuration;
    int currentFrame = 0;

    glm::vec3 translation;
    glm::quat rotation;
    glm::vec3 scale;
public:
    Animation(std::vector<KeyFrame> &keyFrames, bool loop);
    void update(float time);
    void setTransformation(glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
    glm::vec3 getTranslationFrame();
    glm::quat getRotationFrame();
    glm::vec3 getScaleFrame();
    void start();
    void stop();
    int getCurrentFrame();

    bool running;
};


#endif //PUMAROBOT_ANIMATION_H
