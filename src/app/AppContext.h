//
// Created by faliszewskii on 19.04.24.
//

#ifndef PUMAROBOT_APPCONTEXT_H
#define PUMAROBOT_APPCONTEXT_H


#include "../robot/Robot.h"
#include "../room/Room.h"
#include "../cylinder/Cylinder.h"
#include "../mirror/Mirror.h"
#include "../camera/CameraAnchorFree.h"
#include "../framebufferManager/FrameBufferManager.h"
#include "../importer/Importer.h"
#include "../light/PointLight.h"
#include "../skybox/Skybox.h"
#include "../trail/Trail.h"
#include "../point/Point.h"
#include "../flame/Flame.h"
#include "../camera/CameraGameLike.h"
#include "../spark/SparksEntity.h"
#include "../water/Water.h"
#include "../light/DirLight.h"
#include "../seaFloor/SeaFloor.h"
#include "../sea/Sea.h"
#include "stb/stb_perlin.h"
#include "../perlin/Perlin.h"
#include "../sky/Sky.h"
#include "../duck/Duck.h"


struct AppContext {
    std::unique_ptr<DirLight> sunLight;
    std::unique_ptr<Water> water;
    std::unique_ptr<SeaFloor> seaFloor;
    std::unique_ptr<Sea> sea;
    std::unique_ptr<Sky> sky;
    std::unique_ptr<Duck> duck;
    float dropletsPerSec = 5;

    std::unique_ptr<Perlin> perlin;
    std::unique_ptr<Texture> perlinNoise;
    std::unique_ptr<PreethamSkyModel> preethamSkyModel;

    std::unique_ptr<BaseCamera> camera;
    std::unique_ptr<FrameBufferManager> frameBufferManager;

    bool guiFocus = true;

    CameraType cameraType;

    float cameraSensitivity = 0.8f;

    AppContext() : frameBufferManager(), cameraType(CameraType::FREEANCHOR)
    {
        allocateCamera();
        frameBufferManager = std::make_unique<FrameBufferManager>();
        frameBufferManager->create_buffers(camera->screenWidth, camera->screenHeight);
        sunLight = std::make_unique<DirLight>();
        int perlinResolution = 64;

        perlinNoise = std::make_unique<Texture>(perlinResolution, perlinResolution, perlinResolution, 1,GL_RED, GL_RED, GL_FLOAT, GL_TEXTURE_3D);
        std::vector<float> perlinData(perlinResolution * perlinResolution * perlinResolution);
        for(int x=0; x<perlinResolution; x++) {
            for(int y=0; y<perlinResolution; y++) {
                for(int z=0; z<perlinResolution; z++) {
                    int id = x * perlinResolution * perlinResolution + y * perlinResolution + z;
                    perlinData[id] = 0.5f + stb_perlin_noise3(8 * (float)x / perlinNoise->width, 8 * (float)y / perlinNoise->height, 8 * (float)z / perlinNoise->depth, 8, 8, 8);
                }
            }
        }
        perlinNoise->update3D(perlinData.data());
        perlin = std::make_unique<Perlin>();

        water = std::make_unique<Water>(*perlinNoise);
        seaFloor = std::make_unique<SeaFloor>();
        sea = std::make_unique<Sea>(*perlinNoise);
        sky = std::make_unique<Sky>();
        preethamSkyModel = std::make_unique<PreethamSkyModel>();
        preethamSkyModel->set_direction(glm::normalize(glm::vec3({0.2, -1, 0.2})));
        preethamSkyModel->set_turbidity(2);
        preethamSkyModel->update();
        duck = std::make_unique<Duck>();
    }

    void glfw_window_resize(unsigned int width, unsigned int height)
    {
        camera->resize(width, height); // NOLINT(*-narrowing-conversions)
        frameBufferManager->create_buffers(camera->screenWidth, camera->screenHeight);
    }

    void allocateCamera()
    {
        switch(cameraType)
        {
            case CameraType::GAMELIKE:
                if(camera != nullptr)
                    camera.reset();
                camera = std::make_unique<CameraGameLike>(1920, 1080, CameraMode::ANCHOR, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.f), glm::vec3(-M_PI / 4, 0, 0));
                break;
            case CameraType::FREEANCHOR:
                if(camera != nullptr)
                    camera.reset();
                camera = std::make_unique<CameraAnchorFree>(1920, 1080, CameraMode::ANCHOR, glm::vec3(0.0f, 3.0f, 3.0f), glm::vec3(0.f), glm::vec3(-M_PI / 4, 0, 0));
                break;
            default:
                break;
        }
    }

};


#endif //PUMAROBOT_APPCONTEXT_H
