//
// Created by Bartek Jadczak on 27/04/2024.
//

#include "Room.h"

Room::Room ()
{
    generateRoom(1, 1, 1);
    std::vector<std::string> faces
            {
                    "../res/textures/rightC.jpg",    // Right
                    "../res/textures/leftC.jpg",    // Left
                    "../res/textures/top.jpg", // Top
                    "../res/textures/bottomC.jpg",    // Bottom
                    "../res/textures/frontC.jpg",    // Front
                    "../res/textures/backC.jpg"     // Back
            };
    cubemap = std::make_unique<Cubemap>(std::move(faces));
}

void Room::render (Shader &shader)
{
    shader.setUniform("material.hasTexture", true);
    shader.setUniform("material.texture", 0);
    material.setupMaterial(shader);
    shader.setUniform("model", glm::mat4(1));
    cubemap->bind(0);
    wallsMesh->render();

}

void Room::generateRoom (float width, float height, float depth)
{
    std::vector<PosNorTexVertex> wallVertices = {
            // Front face
            {glm::vec3(-width / 2, -height / 2, depth / 2),          glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1, 1)},
            {glm::vec3(width / 2,  -height / 2, depth / 2),           glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0, 1)},
            {glm::vec3(width / 2,   height / 2, depth / 2),   glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0, 0)},
            {glm::vec3(-width / 2,  height / 2, depth / 2),  glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1, 0)},

            // Back face
            {glm::vec3(-width / 2, -height / 2, -depth / 2),         glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec2(0, 1)},
            {glm::vec3(width / 2,  -height / 2, -depth / 2),          glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec2(1, 1)},
            {glm::vec3(width / 2,   height / 2, -depth / 2),  glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec2(1, 0)},
            {glm::vec3(-width / 2,  height / 2, -depth / 2), glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec2(0, 0)},

            // Right face
            {glm::vec3(width / 2, -height / 2, depth / 2),           glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1, 1)},
            {glm::vec3(width / 2, -height / 2, -depth / 2),          glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0, 1)},
            {glm::vec3(width / 2,  height / 2, -depth / 2),  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0, 0)},
            {glm::vec3(width / 2,  height / 2, depth / 2),   glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1, 0)},

            // Left face
            {glm::vec3(-width / 2, -height / 2, depth / 2),          glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec2(0, 1)},
            {glm::vec3(-width / 2, -height / 2, -depth / 2),         glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec2(1, 1)},
            {glm::vec3(-width / 2,  height / 2, -depth / 2), glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec2(1, 0)},
            {glm::vec3(-width / 2,  height / 2, depth / 2),  glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec2(0, 0)},

            // Top face
            {glm::vec3(-width / 2, height / 2, depth / 2),  glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0, 0)},
            {glm::vec3(width / 2,  height / 2, depth / 2),   glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1, 0)},
            {glm::vec3(width / 2,  height / 2, -depth / 2),  glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1, 1)},
            {glm::vec3(-width / 2, height / 2, -depth / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0, 1)},

            // Bottom face
            {glm::vec3(-width / 2, -height / 2, depth / 2), glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec2(0, 1)},
            {glm::vec3(width / 2,  -height / 2, depth / 2), glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec2(1, 1)},
            {glm::vec3(width / 2,  -height / 2, -depth / 2), glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec2(1, 0)},
            {glm::vec3(-width / 2, -height / 2, -depth / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, 0)},
    };

    std::vector<unsigned int> wallIndices = {
            // Front face
            0, 2, 1,
            2, 0, 3,

            // Back face
            4, 5, 6,
            6, 7, 4,

            // Right face
            8, 10, 9,
            10, 8, 11,

            // Left face
            12, 13, 14,
            14, 15, 12,

            // Top face
            16, 18, 17,
            18, 16, 19,

            // Bottom face
            20, 21, 22,
            22, 23, 20,
    };

    // Assign vertices and indices to the model
    wallsMesh = std::make_unique<Mesh<PosNorTexVertex>>(wallVertices, wallIndices);
}
