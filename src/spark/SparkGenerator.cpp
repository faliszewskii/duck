//
// Created by faliszewskii on 07.05.24.
//

#include <random>
#include "SparkGenerator.h"

void SparkGenerator::update(bool generate) {
    if(generate && sparks.size() < MAX_SPARKS) {
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(0, 1);
        std::uniform_real_distribution<float> distDev(1, 2);
        std::uniform_real_distribution<float> distSpeed(0.2, 1);
        std::bernoulli_distribution distBool;
        if(dist(rng) > 0.5) {
            glm::vec3 v1 = glm::normalize(robot.kinematics.needleOrientation);
            glm::vec3 v2{0, 0, 1}; // x
            glm::vec3 v3 = glm::normalize(glm::cross(v1, v2)); // y


            float f2 = dist(rng) * (distBool(rng)?1.f:-1.f);
            float f3 = dist(rng) * (distBool(rng)?1.f:-1.f);
            float speed = 0.02f * distSpeed(rng);
            glm::vec3 dir = glm::normalize(f2 * v2 + f3 * v3);
            float distance = distDev(rng);

            glm::vec3 velocity = (v1 + dir*distance) * speed;

            sparks.push_back(Spark{
                    {},
                    robot.kinematics.needlePosition,
                    velocity,
                    MAX_TTL});
        }
    }

    float timeDiff = glfwGetTime() - prevTime;
    for(auto &spark : sparks) {
        spark.ttl -= timeDiff;
        spark.prevPos = spark.currPos;
        spark.currPos += spark.velocity;
        spark.velocity += glm::vec3(0, -9.81, 0)*0.0005f;
    }
    prevTime = glfwGetTime();
    sparks.erase(std::remove_if(sparks.begin(), sparks.end(), [&](const auto &item) {
        return item.ttl < 0;
    }), sparks.end());
}
