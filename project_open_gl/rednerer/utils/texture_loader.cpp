//
// Created by Andrii Krenevych on 08.04.2025.
//
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glad/glad.h"

#include "texture_loader.h"

#include <memory>

#include "file_utils.h"
#include "Logger.h"
#include "assimp/Logger.hpp"
#include "assimp/scene.h"
#include "assimp/texture.h"


std::shared_ptr<Renderer::Texture> createTexture(const unsigned char* data, int width, int height) {
    if (!data) {
        return nullptr;
    }

    // засилання даних на відеокарту та отримання id текстури
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    auto texture = std::make_shared<Renderer::Texture>(textureId, width, height);
    return texture;
}

std::shared_ptr<Renderer::Texture> loadTexture(const std::string& fileName) {
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        Renderer::Logger::log().e("Failed to load texture from file: " + fileName);
        return nullptr;
    }

    auto texture = createTexture(data, width, height);

    stbi_image_free(data);
    return texture;
}

std::shared_ptr<Renderer::Texture> loadEmbeddedTexture(const aiTexture* tex) {
    if (tex->mHeight == 0) {
        int width, height, channels;
        unsigned char* data = stbi_load_from_memory(
            reinterpret_cast<unsigned char*>(tex->pcData),
            tex->mWidth,
            &width, &height, &channels, 0
        );

        if (!data) {
            Renderer::Logger::log().e("Failed to load texture from model file: ");
            return nullptr;
        }

        auto texture = createTexture(data, width, height);
        stbi_image_free(data);
        return texture;
    }

    Renderer::Logger::log().e("Unsupported uncompressed embedded texture");
    return nullptr;
}

std::shared_ptr<Renderer::Texture> loadTextureFromAssimp(const aiScene* scene,
                                                         const aiMaterial* material,
                                                         aiTextureType type,
                                                         const std::string& modelDir) {
    aiString path;
    if (material->GetTexture(type, 0, &path) != AI_SUCCESS) {
        return nullptr; // текстура не задана
    }

    Renderer::Logger::log().d("Found texture: " + std::string(path.C_Str()));

    std::string texPath = path.C_Str();

    if (!texPath.empty() && texPath[0] == '*') {
        int index = std::atoi(texPath.c_str() + 1);
        if (index < 0 || index >= static_cast<int>(scene->mNumTextures)) return nullptr;

        const aiTexture* tex = scene->mTextures[index];
        return loadEmbeddedTexture(tex);
    }

    normalizePath(texPath);

    std::string fullPath = modelDir + PATH_SEPARATOR + texPath;
    return loadTexture(fullPath);
}
