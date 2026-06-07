//
// Created by Andrii Krenevych on 21.04.2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>

namespace Renderer {

class Texture {
public:
    explicit Texture(unsigned int textureId, unsigned int width, unsigned int height);
    virtual ~Texture();

    void bind(unsigned int shaderProgram, const std::string &name, unsigned int textureUnit) const;
    static void unbind();

private:
    unsigned int mTextureId;

    unsigned int mWidth;
    unsigned int mHeight;

};

} // Renderer

#endif //TEXTURE_H
