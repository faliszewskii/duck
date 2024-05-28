//
// Created by faliszewskii on 20.05.24.
//

#include <cmath>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include "Duck.h"
#include "glm/gtc/quaternion.hpp"

Duck::Duck() {

    std::ifstream file("../res/models/duck.txt");
    if (!file.is_open()) {
        throw(std::runtime_error("Error opening file."));
    }

    std::vector<std::string> tokens;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
    }

    int caret = 0;

    // Vertices section starts with their count.
    int vertexCount = std::stoi(tokens[caret++]);
    // Load vertices
    std::vector<PosNorTexVertex> vertices(vertexCount);
    for(int i=0; i < vertexCount; i++) {
        glm::vec3 position(std::stof(tokens[caret + 0]), std::stof(tokens[caret + 1]), std::stof(tokens[caret + 2]));
        caret+=3;
        glm::vec3 normal(std::stof(tokens[caret+0]), std::stof(tokens[caret+1]), std::stof(tokens[caret+2]));
        caret+=3;
        glm::vec2 texCoords(std::stof(tokens[caret+0]), std::stof(tokens[caret+1]));
        caret+=2;
        vertices[i].position = position;
        vertices[i].normal = normal;
        vertices[i].texCoords = texCoords;
    }

    // Indices section starts with their count.
    int indexCount = std::stoi(tokens[caret++]);
    // Load vertices
    std::vector<unsigned int> indices(indexCount * 3);
    for(int i=0; i < 3*indexCount; i++) {
        indices[i] =  std::stoul(tokens[caret + i]);
    }

    mesh = std::make_unique<Mesh<PosNorTexVertex>>(vertices, indices);

    duckScale = glm::vec3( 0.0005);

    std::vector<KeyFrame> keyFrames;
    keyFrames.push_back({KeyFrame::LINEAR, 0.5, glm::vec3(0), glm::identity<glm::quat>(), glm::vec3(1.25,0.8,0.8)});
    keyFrames.push_back({KeyFrame::LINEAR, 0.5, glm::vec3(0), glm::identity<glm::quat>(), glm::vec3(1)});
    animation = std::make_unique<Animation>(keyFrames, true);

    trackControlPoints[trackControlPoints.size()-1] = glm::vec2(0.25, 0);
    deBoorPoints[deBoorPoints.size()-1].position = {
            trackControlPoints[trackControlPoints.size()-1].x,
            0,
            trackControlPoints[trackControlPoints.size()-1].y
    };
    minPoints[minPoints.size()-1].position = {
            trackMin,
            0.01f,
            0
    };
    maxPoints[minPoints.size()-1].position = {
            trackRadius,
            0.01,
            0
    };
    generateNextTrack();
    generateNextTrack();
    generateNextTrack();
    generateNextTrack();
    generateNextTrack();
    lastTime = glfwGetTime();
}

void Duck::render(Shader &shader) {
    shader.setUniform("material.hasTexture", true);
    shader.setUniform("material.texture", 0);
    texture.bind(0);
    shader.setUniform("material.shininess", 512.f);
    shader.setUniform("anisotropicTexture", 1);
    anisotropicTexture.bind(1);

    auto model = glm::identity<glm::mat4>();
    model = glm::translate(model, duckPosition);
    model = glm::translate(model, glm::vec3(0, -submersion, 0));
    model = glm::translate(model, animation->getTranslationFrame());
    model = glm::mat4_cast(animation->getRotationFrame()) * model;
    model = glm::rotate(model, duckRotation, glm::vec3(0,1,0));
    model = glm::scale(model, duckScale);
    model = glm::scale(model, animation->getScaleFrame());
    shader.setUniform("model", model);

    mesh->render();
}

void Duck::renderDeBoorPoints(Shader &shader) {
    glPointSize(6);
    for(auto &point : deBoorPoints)
        point.render(shader);
//    glPointSize(3);
    for(int i=0; i<trackControlPoints.size(); i++) {
        glm::vec2 vector = {std::cos(trackAngle - (float)(trackControlPoints.size() - i - 1) * trackAngleStep), std::sin(trackAngle - (float)(trackControlPoints.size() - i - 1) * trackAngleStep)};

        minPoints[i].color = glm::vec4(0.2, 0.5, 0.1, 1);
        maxPoints[i].color = glm::vec4(0.2, 0.5, 0.1, 1);
        minPoints[i].position = {vector.x * trackMin, 0.01f, vector.y * trackMin};
        maxPoints[i].position = {vector.x * trackRadius, 0.01f, vector.y * trackRadius};
        minPoints[i].render(shader);
        maxPoints[i].render(shader);
    }
}

void Duck::renderTrackRadii(Shader &shader) {
    for(int i=0; i<trackControlPoints.size(); i++) {
        glm::vec2 vector = {std::cos(trackAngle - (float)(trackControlPoints.size() - i) * trackAngleStep), std::sin(trackAngle - (float)(trackControlPoints.size() - i) * trackAngleStep)};

        trackRadii[i].positionStart = {vector.x * trackRadius * trackMin, 0.01f, vector.y * trackRadius * trackMin};
        trackRadii[i].positionEnd = {vector.x * trackRadius, 0.01f, vector.y * trackRadius};
//        trackRadii[i].render(shader);

        glm::vec2 vectorNext = {std::cos(trackAngle - (float)(trackControlPoints.size() - i - 1) * trackAngleStep), std::sin(trackAngle - (float)(trackControlPoints.size() - i - 1) * trackAngleStep)};

        float nearest = fmax(trackMin, glm::length(trackControlPoints[i]) - trackDeviation);
        float farthest = fmin(glm::length(trackControlPoints[i]) + trackDeviation, trackRadius);

        trackRadii[i].positionStart = {vectorNext.x * nearest, 0.01f, vectorNext.y * nearest};
        trackRadii[i].positionEnd = {vectorNext.x * farthest, 0.01f, vectorNext.y * farthest};
        trackRadii[i].color = glm::vec4(0.3, 0.6, 0.2, 1);
        glDepthFunc(GL_LEQUAL);
        trackRadii[i].render(shader);
        glDepthFunc(GL_LESS);
        trackRadii[i].color = glm::vec4(1);
    }
}

void Duck::renderTrackCurve() {
    trackCurve.render();
}

glm::vec3 Duck::getPosition() {
    return duckPosition;
}

void Duck::update(float time) {
    animation->update(time);

    if(trackParameter >= 1) {
        // TODO Do a flip !
        trackParameter = 0;
        generateNextTrack();
    }
    duckPosition = calculatePosition(trackParameter);
    duckRotation = calculateRotation(trackParameter);

    trackParameter += speed * (time - lastTime);
    lastTime = time;
}

void Duck::generateNextTrack() {
    for(int i = 1; i < trackControlPoints.size(); i++) {
        trackControlPoints[i-1] = trackControlPoints[i];
        deBoorPoints[i-1].position = deBoorPoints[i].position;
    }

    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(
            fmax(trackMin, glm::length(trackControlPoints[trackControlPoints.size()-1]) - trackDeviation),
            fmin(glm::length(trackControlPoints[trackControlPoints.size()-1]) + trackDeviation, trackRadius)
            );

    float distanceFromMiddle = dist(rng);

    glm::vec2 vector = glm::vec2(std::cos(trackAngle),std::sin(trackAngle));
    glm::vec2 generatedPoint = vector * distanceFromMiddle;

    trackControlPoints[trackControlPoints.size()-1] = generatedPoint;

    deBoorPoints[deBoorPoints.size()-1].position = {generatedPoint.x, 0.01f, generatedPoint.y};

    std::vector<PositionVertex> bSplinePoints;
    for(int i=0; i< trackControlPoints.size()-3; i++)
        for(int j = 0; j<4; j++) {
            auto point = trackControlPoints[i+j];
            bSplinePoints.push_back({{point.x, 0.01f, point.y}});
        }
    trackCurve.update(bSplinePoints);

    trackAngle += (float)fmod(trackAngleStep, 2 * M_PI);
}

glm::vec3 Duck::deBoorCoefficients3(float t) {
    float N01 = 1;

    float N10 = N01 * (1 - t);
    float N11 = N01 * (t - 0);

    float N2_1 = N10 * (1 - t) / 2.f;
    float N20 = N10 * (t - (-1)) / 2.f + N11 * (2 - t) / 2.f;
    float N21 = N11 * (t - 0) / 2.f;

    return {N2_1, N20, N21};
}

glm::vec4 Duck::deBoorCoefficients4(float t) {
    float N01 = 1;

    float N10 = N01 * (1 - t);
    float N11 = N01 * (t - 0);

    float N2_1 = N10 * (1 - t) / 2.f;
    float N20 = N10 * (t - (-1)) / 2.f + N11 * (2 - t) / 2.f;
    float N21 = N11 * (t - 0) / 2.f;

    float N3_2 = N2_1 * (1 - t) / 3.f;
    float N3_1 = N2_1 * (t - (-2)) / 3.f + N20 * (2 - t) / 3.f;
    float N30 = N20 * (t - (-1)) / 3.f + N21 * (3 - t) / 3.f;
    float N31 = N21 * (t - 0) / 3.f;

    return {N3_2, N3_1, N30, N31};
}

glm::vec3 Duck::calculatePosition(float t) {

    glm::vec4 deBoor = deBoorCoefficients4(t);
    auto position = glm::vec2(0.0);
    for(int i = 0; i < 4; i++)
        position += trackControlPoints[i] * deBoor[i];
    return {position.x, 0, position.y};
}

float Duck::calculateRotation(float t) {
    glm::vec3 deBoor = deBoorCoefficients3(t);
    auto derivative = glm::vec2(0.0);
    for(int i = 0; i < 3; i++)
        derivative += (trackControlPoints[i+1] - trackControlPoints[i]) * deBoor[i];
    derivative = glm::normalize(derivative);
    return atan2(derivative.y, -derivative.x);
}

float Duck::getRotation() {
    return duckRotation;
}
