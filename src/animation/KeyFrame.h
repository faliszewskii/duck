//
// Created by faliszewskii on 21.05.24.
//

#ifndef PUMAROBOT_KEYFRAME_H
#define PUMAROBOT_KEYFRAME_H

#include "KeyFrame.h"
#include "glm/ext/quaternion_float.hpp"

struct KeyFrame {
    enum Interpolation {
        LINEAR
    };

    Interpolation interpolation;
    float duration;

    glm::vec3 translation;
    glm::quat rotation;
    glm::vec3 scale;
};

#endif //PUMAROBOT_KEYFRAME_H
