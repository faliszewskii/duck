//
// Created by faliszewskii on 18.04.24.
//

#ifndef PUMAROBOT_CAMERAANCHORFREE_H
#define PUMAROBOT_CAMERAANCHORFREE_H

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <map>
#include "baseCamera.h"




class CameraAnchorFree : public BaseCamera {
private:
    CameraMode cameraType;

    glm::quat orientation;

    float movementSpeed;
    float mouseSensitivity;
    float zoomSensitivity; // TODO Maybe change based on how close anchor to
    glm::vec3 front{};
    glm::vec3 up{};
    glm::vec3 right{};

public:
    glm::vec3 position;
    glm::vec3 anchor;
    float nearPlane;
    float farPlane;
    float fov;

    bool stereoscopicVision = false;
    float stereoscopicIOD = 0.068;
    float stereoscopicDistance = 9;

    std::map<CameraMode, void (CameraAnchorFree::*)(float, float)> mouseHandlerMapping{
            {CameraMode::FREE,   &CameraAnchorFree::processMouseMovementFree},
            {CameraMode::ANCHOR, &CameraAnchorFree::processMouseMovementAnchor}
    };
    std::map<CameraMode, void (CameraAnchorFree::*)(CameraMovement direction, float)> keyboardHandlerMapping{
            {CameraMode::FREE,   &CameraAnchorFree::processKeyboardFree},
            {CameraMode::ANCHOR, &CameraAnchorFree::processKeyboardAnchor}
    };

    explicit CameraAnchorFree(
            int screenWidth,
            int screenHeight,
            CameraMode cameraMode = FREE,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 anchor = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat orientation = glm::quat(1, 0, 0, 0)
    );

    [[nodiscard]] glm::mat4 getViewMatrix() const override;
    [[nodiscard]] glm::mat4 getNoTranslationViewMatrix() const override;
    [[nodiscard]] glm::mat4 getMirrorViewMatrix() const override;
    [[nodiscard]] glm::mat4 getProjectionMatrix() const override;
    [[nodiscard]] glm::vec3 getViewPosition() const override;
    [[nodiscard]] float getFarPlane() const override;
    [[nodiscard]] float getNearPlane() const override;

    void processKeyboard(CameraMovement direction, float deltaTime) override;
    void processMouseMovement(float xoffset, float yoffset) override;
    void processMouseScroll(float yoffset) override;

    void resize(int screenWidth, int screenHeight) override;

private:

    void processMouseMovementFree(float xoffset, float yoffset);
    void processMouseMovementAnchor(float xoffset, float yoffset);

    void processKeyboardFree(CameraMovement direction, float deltaTime);
    void processKeyboardAnchor(CameraMovement direction, float deltaTime);

    void updateDirections();

};

#endif //PUMAROBOT_CAMERAANCHORFREE_H
