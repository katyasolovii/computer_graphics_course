//
// Created by Andrii Krenevych on 21.04.2025.
//

#include "Transformation.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include <glm/gtx/matrix_decompose.hpp>

namespace Renderer {
    Transformation::Transformation()
        : mPosition(0.0f, 0.0f, 0.0f),
          mRotation(1.0, 0.0, 0.0, 0.0),
          mScale(1.0f) {
    }

    glm::mat4 Transformation::getTransform() const {
        const glm::mat4 translate = glm::translate(glm::mat4(1.0f), mPosition);
        const glm::mat4 rotate = glm::toMat4(mRotation);
        const glm::mat4 scaleM = glm::scale(glm::mat4(1.0f), mScale);
        return translate * rotate * scaleM;
    }

    void Transformation::setPosition(const glm::vec3& position) {
        mPosition = position;
    }

    void Transformation::setScale(const glm::vec3& scale) {
        mScale = scale;
    }

    void Transformation::setRotationEuler(glm::vec3 eulerDegrees) {
        const glm::vec3 radians = glm::radians(eulerDegrees);
        mRotation = glm::quat(radians);
    }

    void Transformation::setRotation(const glm::quat& quat) {
        mRotation = quat;
    }

    void Transformation::setRotation(const glm::mat4& matrix) {
        glm::vec3 skew, scale, position;
        glm::vec4 perspective;
        glm::decompose(matrix, scale, mRotation, position, skew, perspective);
        mRotation = glm::normalize(mRotation);
    }

    void Transformation::setTransform(const glm::mat4& matrix) {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(matrix, mScale, mRotation, mPosition, skew, perspective);
        mRotation = glm::normalize(mRotation);
    }

    const glm::vec3& Transformation::getPosition() const {
        return mPosition;
    }

    const glm::vec3& Transformation::getScale() const {
        return mScale;
    }

    const glm::quat& Transformation::getRotation() const {
        return mRotation;
    }
} // Renderer
