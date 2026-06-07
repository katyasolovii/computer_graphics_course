//
// Created by Andrii Krenevych on 21.04.2025.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Node.h"

namespace Renderer {
    class Camera : public Node {
    public:
        Camera(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);

        void setLookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 getViewMatrix() const;

        glm::mat4 getProjectionMatrix() const;

        void applyClear() const;

        void onViewportChange(int width, int height);

        void setClearColor(const glm::vec4& color);

        void setClearColor(float r, float g, float b, float a);

        void setClearColorBufferEnabled(bool enabled);

        void setClearDepthBufferEnabled(bool enabled);

        void setDepthTestEnabled(bool enabled);

        const glm::vec4& getClearColor() const;

        bool isClearColorBufferEnabled() const;

        bool isClearDepthBufferEnabled() const;

        bool isDepthTestEnabled() const;

        void applyDepthTest() const;

        void setDrawOrder(int order);

        int getDrawOrder() const;

        void setRenderMask(long mask);

        long getRenderMask() const;

        void rotate(float yawDeg, float pitchDeg) const;

        void moveLocal(glm::vec3 delta) const;

        void moveForward(float x) const;

        void moveRight(float distance) const;

        void moveUp(float distance) const;

    private:
        int mDrawOrder = 0;
        long mRenderMask = -1;
        // бітова маска видимості, за промовчанням -1, що відповідає числу "11111...111" у двійковій системі запису


        glm::vec4 mClearColor{1.0f, 1.0f, 1.0f, 1.0f};
        bool mClearColorBuffer = true;
        bool mClearDepthBuffer = true;
        bool mEnableDepthTest = true;

        float mFovDegrees;
        float mAspectRatio;
        float mNear;
        float mFar;
    };
} // Renderer

#endif //CAMERA_H
