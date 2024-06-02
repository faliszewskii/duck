//
// Created by Bartek Jadczak on 29/05/2024.
//

#include "Cubemap.h"
#include "../Shader.h"
#include "common.h"

#include <random>

Cubemap::Cubemap (
        std::vector<std::string> faces
)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);


    for (unsigned int i = 0; i < faces.size(); i++)
    {
        int width, height, nrChannels;
        unsigned char *data = TextureCommon::loadImage(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    nrChannels==4 ? GL_SRGB_ALPHA : GL_SRGB,
                    width,
                    height,
                    0,
                    nrChannels==4 ? GL_RGBA : GL_RGB,
                    GL_UNSIGNED_BYTE,
                    data
            );
            glGenerateMipmap(id);
            TextureCommon::freeImage(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            TextureCommon::freeImage(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::bind (int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}
