//
// Created by faliszewskii on 18.04.24.
//

#ifndef PUMAROBOT_SCENE_H
#define PUMAROBOT_SCENE_H


#include "robot/Robot.h"
#include "camera/CameraAnchorFree.h"
#include "app/AppContext.h"

class Scene {

    Shader pointShader;
    Shader lineShader;
    Shader bSplineShader;
    Shader waterShader;
    Shader phongShader;
    Shader infiniteShader;
    Shader seaShader;
    Shader skyShader;

    Shader waterHeightCompute;

    AppContext &appContext;

public:
    explicit Scene(AppContext &appContext);
    void update();
    void render();
};


#endif //PUMAROBOT_SCENE_H
