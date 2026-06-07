//
// Created by Andrii Krenevych on 21.04.2025.
//

#include "Property.h"

#include <utility>

#include "glad/glad.h"

namespace Renderer {
    Property::Property(std::string name)
        : mName(std::move(name)) {
    }

    const std::string& Property::getName() const {
        return mName;
    }

    GLint Property::location(const unsigned int shaderProgram) const {
        return glGetUniformLocation(shaderProgram, mName.c_str());
    }

    /*   PropertyInt   */
    PropertyInt::PropertyInt(std::string name, int value)
        : Property(std::move(name)),
          mValue(value) {
    }

    void PropertyInt::bind(unsigned int shaderProgram) {
        glUniform1i(location(shaderProgram), mValue);
    }

    /*   PropertyFloat   */
    PropertyFloat::PropertyFloat(std::string name, const float value)
        : Property(std::move(name)),
          mValue(value) {
    }

    void PropertyFloat::bind(unsigned int shaderProgram) {
        glUniform1f(location(shaderProgram), mValue);
    }

    /*   PropertyVec3   */
    PropertyVec3::PropertyVec3(std::string name,
                               const float x, const float y, const float z)
        : PropertyVec3(std::move(name), glm::vec3(x, y, z)) {
    }

    PropertyVec3::PropertyVec3(std::string name, const glm::vec3& value)
        : Property(std::move(name)),
          mValue(value) {
    }

    void PropertyVec3::bind(const unsigned int shaderProgram) {
        glUniform3fv(location(shaderProgram), 1, (float*) &mValue);
    }

    /*   PropertyVec4   */
    PropertyVec4::PropertyVec4(std::string name,
                               const float x, const float y, const float z, const float w)
        : PropertyVec4(std::move(name), glm::vec4(x, y, z, w)) {
    }

    PropertyVec4::PropertyVec4(std::string name, const glm::vec4& value)
        : Property(std::move(name)),
          mValue(value) {
    }

    void PropertyVec4::bind(const unsigned int shaderProgram) {
        glUniform4fv(location(shaderProgram), 1, (float*) &mValue);
    }

    /*   PropertyMat4   */
    PropertyMat4::PropertyMat4(std::string name, const glm::mat4& value)
        : Property(std::move(name)),
          mValue(value) {
    }

    void PropertyMat4::bind(const unsigned int shaderProgram) {
        glUniformMatrix4fv(location(shaderProgram), 1, GL_FALSE, (float*) &mValue);
    }
} // Renderer
