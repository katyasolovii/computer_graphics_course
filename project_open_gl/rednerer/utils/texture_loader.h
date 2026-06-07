//
// Created by Andrii Krenevych on 08.04.2025.
//

#pragma once

#include <memory>
#include  <string>
#include  <Texture.h>

#include "assimp/material.h"
#include "assimp/texture.h"


struct aiMaterial;
struct aiScene;

std::shared_ptr<Renderer::Texture> createTexture(const unsigned char* data, int width, int height);

std::shared_ptr<Renderer::Texture> loadTexture(const std::string& fileName);

std::shared_ptr<Renderer::Texture> loadEmbeddedTexture(const aiTexture* tex);

std::shared_ptr<Renderer::Texture> loadTextureFromAssimp(const aiScene* scene,
                                                         const aiMaterial* material,
                                                         aiTextureType type,
                                                         const std::string& modelDir);

// #endif //TEXTURE_H
