//
// Created by Andrii Krenevych on 21.04.2025.
//

#include "Camera.h"

#include "Constants.h"
#include "Transformation.h"
#include "glad/glad.h"

namespace Renderer {
    Camera::Camera(const float fovDegrees, const float aspectRatio, const float nearPlane, const float farPlane)
        : Node(DEFAULT_CAMERA_NAME),
          mFovDegrees(fovDegrees),
          mAspectRatio(aspectRatio),
          mNear(nearPlane),
          mFar(farPlane) {
    }

    void Camera::setLookAt(const glm::vec3& target, const glm::vec3& up) {
        auto position = glm::vec3(mTransform->getTransform()[3]); // поточна позиція камери
        glm::mat4 view = glm::lookAt(position, target, up);
        mTransform->setTransform(glm::inverse(view));
    }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::inverse(mTransform->getTransform());
    }

    glm::mat4 Camera::getProjectionMatrix() const {
        return glm::perspective(glm::radians(mFovDegrees), mAspectRatio, mNear, mFar);;
    }

    void Camera::setClearColor(const glm::vec4& color) {
        mClearColor = color;
    }

    void Camera::setClearColor(const float r, const float g, const float b, const float a) {
        mClearColor = glm::vec4(r, g, b, a);
    }

    void Camera::setClearColorBufferEnabled(const bool enabled) {
        mClearColorBuffer = enabled;
    }

    void Camera::setClearDepthBufferEnabled(const bool enabled) {
        mClearDepthBuffer = enabled;
    }

    void Camera::setDepthTestEnabled(const bool enabled) {
        mEnableDepthTest = enabled;
    }

    const glm::vec4& Camera::getClearColor() const {
        return mClearColor;
    }

    bool Camera::isClearColorBufferEnabled() const {
        return mClearColorBuffer;
    }

    bool Camera::isClearDepthBufferEnabled() const {
        return mClearDepthBuffer;
    }

    bool Camera::isDepthTestEnabled() const {
        return mEnableDepthTest;
    }

    void Camera::applyClear() const {
        glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);

        GLbitfield clearFlags = 0;
        if (mClearColorBuffer) clearFlags |= GL_COLOR_BUFFER_BIT;
        if (mClearDepthBuffer) clearFlags |= GL_DEPTH_BUFFER_BIT;

        if (clearFlags != 0)
            glClear(clearFlags);
    }

    void Camera::onViewportChange(int width, int height) {
        mAspectRatio = (float) width / (float) height;
    }

    void Camera::applyDepthTest() const {
        if (mEnableDepthTest) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void Camera::setDrawOrder(int order) {
        mDrawOrder = order;
    }

    int Camera::getDrawOrder() const {
        return mDrawOrder;
    }

    void Camera::setRenderMask(long mask) {
        mRenderMask = mask;
    }

    long Camera::getRenderMask() const {
        return mRenderMask;
    }

    void Camera::rotate(float yawDeg, float pitchDeg) const {
        const auto transform = mTransform->getTransform();
        const auto rotation = mTransform->getRotation();

        // 2. Обчислюємо обертання
        const glm::quat qPitch = glm::angleAxis(glm::radians(pitchDeg), glm::vec3(transform[0])); // локальна X (right)
        const glm::quat qYaw = glm::angleAxis(glm::radians(yawDeg), glm::vec3(0.0f, 1.0f, 0.0f)); // глобальна Y

        // 4. Нова орієнтація: Yaw * Pitch * Current
        const glm::quat newRotation = glm::normalize(qYaw * qPitch * rotation);

        mTransform->setRotation(newRotation);
    }

    void Camera::moveLocal(glm::vec3 delta) const {
        auto transform = mTransform->getTransform();

        const auto position = glm::vec3(transform[3]);
        const auto forward = -glm::normalize(glm::vec3(transform[2])); // -Z
        const auto up = glm::normalize(glm::vec3(transform[1])); // Y
        const auto right = glm::normalize(glm::cross(forward, up));

        const auto lookAt = position + forward;

        const auto move = delta.x * right + delta.y * up + delta.z * forward;
        const auto newPos = position + move;
        const auto newLookAt = lookAt + move;
        const glm::mat4 view = glm::lookAt(newPos, newLookAt, up);
        mTransform->setRotation(glm::inverse(view));
        mTransform->setPosition(newPos);
    }

    void Camera::moveForward(float delta) const {
        moveLocal({0.0, 0.0, delta});
    }

    void Camera::moveRight(float delta) const {
        moveLocal({delta, 0.0, 0.0});
    }

    void Camera::moveUp(float delta) const {
        moveLocal({0.0, delta, 0.0});
    }
} // Renderer
