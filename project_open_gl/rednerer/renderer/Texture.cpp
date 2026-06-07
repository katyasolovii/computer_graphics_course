//
// Created by Andrii Krenevych on 21.04.2025.
//

#include <string>
#include <iostream>

#include "glad/glad.h"

#include "Texture.h"

namespace Renderer {

    Texture::Texture(unsigned int textureId, unsigned int width, unsigned int height)
        : mTextureId(textureId),
          mWidth(width),
          mHeight(height) {
    }

    Texture::~Texture() {
        if (mTextureId) {
            glDeleteTextures(1, &mTextureId);
        }
    }

    void Texture::bind(unsigned int shaderProgram, const std::string &name, unsigned int textureUnit) const {
        auto texture0_loc = glGetUniformLocation(shaderProgram, name.c_str()); // (char *)&name
        glActiveTexture(GL_TEXTURE0 + textureUnit); // активує текстурний юніт під номером textureUnit
        glBindTexture(GL_TEXTURE_2D, mTextureId);
        glUniform1i(texture0_loc, (int) textureUnit);
    }

    void Texture::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
} // Renderer
