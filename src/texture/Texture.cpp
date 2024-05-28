//
// Created by faliszewskii on 06.05.24.
//

#include <vector>
#include "Texture.h"
#include "../Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Texture::Texture(const std::string& path) {
    this->target = GL_TEXTURE_2D;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    format = nrChannels==4 ? GL_RGBA : GL_RGB;
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            nrChannels==4 ? GL_SRGB_ALPHA : GL_SRGB,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
    );
    glGenerateMipmap(id);
    stbi_image_free(data);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(int x, int y, int nrChannels, int internalFormat, int format, int type, int target) {
    width = x;
    height = y;
    this->format = format;
    this->internalFormat = internalFormat;
    this->target = target;
    this->nrChannels = nrChannels;
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(target, id);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    switch (target) {
        case GL_TEXTURE_2D:
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, format, type, nullptr);
            break;
        default:
            break;
    }
    glBindImageTexture(0, id, 0, GL_FALSE, 0, GL_READ_WRITE, format);

    glGenerateMipmap(id);
}

void Texture::bind(int slot) const {
    glBindTextureUnit(slot, id);
}

void Texture::bindImage(int slot) const {
    glBindTextureUnit(slot, id);
    glBindImageTexture(slot, id, 0, GL_FALSE, 0, GL_READ_WRITE, internalFormat);
}

void Texture::update2D(void *data) const {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        width,
        height,
        GL_RGBA,
        GL_FLOAT,
        data
    );
}

void Texture::update3D(void *data) const {
    glBindTexture(target, id);
    glTexSubImage3D(
            target,
            0,
            0,
            0,
            0,
            width,
            height,
            depth,
            format,
            GL_FLOAT,
            data
    );
}

Texture::Texture(int x, int y, int z, int nrChannels, int internalFormat, int format, int type, int target) {
    width = x;
    height = y;
    depth = z;
    this->format = format;
    this->internalFormat = internalFormat;
    this->target = target;
    this->nrChannels = nrChannels;
    glGenTextures(1, &id);
    glBindTexture(target, id);

    switch (target) {
        case GL_TEXTURE_3D:
            glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, x, y, z, 0, format, type, nullptr);
            break;
        default:
            break;
    }

    glGenerateMipmap(id);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_REPEAT);
}

Texture::Texture(const std::string &path, int repeat) : Texture(path) {
    glTexParameteri(target, GL_TEXTURE_WRAP_S, repeat);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, repeat);
}

void Texture::updatePixel(int x, int y, void *data) const {
    glBindTexture(target, id);
    glTexSubImage2D(
            target,
            0,
            x,
            y,
            1,
            1,
            format,
            GL_FLOAT,
            data
    );
}
