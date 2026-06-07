//
// Created by Andrii Krenevych on 21.04.2025.
//

#include "Material.h"

#include <utility>

#include "Constants.h"
#include "Logger.h"

namespace Renderer {
    Material::Material(std::string name)
        : mName(std::move(name)),
          mProgram(nullptr) {

        Logger::log().d("Create Material: " + mName);
    }

    Material::Material()
        : Material(DEFAULT_MATERIAL_NAME) {
    }

    Material::~Material() = default;

    void Material::activateProgram() const {
        mProgram->activate();
    }

    void Material::bind() const {
        Logger::log().d("Material::bind: " + mName);

        int textureUnit = 0;
        for (const auto& textureEntity: mTextures) {
            const auto& name = textureEntity.first;
            const auto& texture = textureEntity.second;
            texture->bind(mProgram->getShaderProgram(), name, textureUnit++);
        }

        for (const auto& propertyEntity: mProperties) {
            const auto& name = propertyEntity.first;
            const auto& property = propertyEntity.second;
            property->bind(mProgram->getShaderProgram());
        }
    }

    void Material::unbind() const {
        Logger::log().d("Material::unbind: " + mName);

        mProgram->deactivate();
    }

    void Material::setProgram(const std::shared_ptr<Program>& program) {
        mProgram = program;
    }

    const std::shared_ptr<Program>& Material::getProgram() const {
        return mProgram;
    }

    void Material::setTexture(const std::string& name, const std::shared_ptr<Texture>& texture) {
        mTextures[name] = texture;
    }

    void Material::setProperty(const std::shared_ptr<Property>& property) {
        mProperties[property->getName()] = property;
    }

    void Material::setProperty(const std::string& name, const glm::mat4& value) {
        mProperties[name] = std::make_shared<PropertyMat4>(name, value);
    }

    void Material::setProperty(const std::string& name, const glm::vec3& value) {
        mProperties[name] = std::make_shared<PropertyVec3>(name, value);
    }

    void Material::setProperty(const std::string& name, const glm::vec4& value) {
        mProperties[name] = std::make_shared<PropertyVec4>(name, value);
    }

    void Material::setProperty(const std::string& name, float value) {
        mProperties[name] = std::make_shared<PropertyFloat>(name, value);
    }

    void Material::setProperty(const std::string& name, int value) {
        mProperties[name] = std::make_shared<PropertyInt>(name, value);
    }

    bool Material::hasTexture(const std::string& str) const {
        return mTextures.find(str) != mTextures.end();
    }

    bool Material::hasProperty(const std::string& str) const {
        return mProperties.find(str) != mProperties.end();
    }

} // Renderer
