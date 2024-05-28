//
// Created by faliszewskii on 20.05.24.
//

#ifndef PUMAROBOT_DUCK_H
#define PUMAROBOT_DUCK_H


#include "../mesh/PosNorTexVertex.h"
#include "../texture/Texture.h"
#include "../mesh/Mesh.h"
#include "../animation/Animation.h"
#include "../point/Point.h"
#include "../line/Line.h"
#include "../bspline/Bspline.h"

class Duck {

    float trackParameter = 0;
    std::array<glm::vec2, 6> trackControlPoints{};
    float lastTime=0;
    float trackAngle = 0.5;

    std::array<Point, 6> deBoorPoints{};
    std::array<Point, 6> minPoints{};
    std::array<Point, 6> maxPoints{};
    std::array<Line, 6> trackRadii{};
    BSpline trackCurve;

    std::unique_ptr<Mesh<PosNorTexVertex>> mesh;
    Texture texture{"../res/textures/ducktex.jpg"};
    Texture anisotropicTexture{"../res/textures/anisotropic.png"};

    glm::vec3 duckScale;
    float duckRotation;
    glm::vec3 duckPosition{};
public:
    float submersion = 0.01f;
    float speed = 0.5;
    float trackMin = 0.15;
    float trackRadius = 0.5;
    float trackAngleStep = 0.5;
    float trackDeviation = 0.15;

    std::unique_ptr<Animation> animation;
    bool debugDuckTruck = false;

    Duck();

    void update(float time);
    void render(Shader &shader);
    void renderDeBoorPoints(Shader &shader);
    void renderTrackRadii(Shader &shader);
    void renderTrackCurve();

    glm::vec3 getPosition();
    float getRotation();

    void generateNextTrack();
    glm::vec3 calculatePosition(float parameter);
    float calculateRotation(float t);

    glm::vec3 deBoorCoefficients3(float t);
    glm::vec4 deBoorCoefficients4(float t);
};


#endif //PUMAROBOT_DUCK_H
