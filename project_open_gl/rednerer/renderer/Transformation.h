//
// Created by Andrii Krenevych on 21.04.2025.
//

#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


namespace Renderer {
    class Transformation {
    public:
        Transformation();

        void setPosition(const glm::vec3& position);

        void setScale(const glm::vec3& scale);

        void setRotationEuler(glm::vec3 eulerDegrees);

        void setRotation(const glm::quat& quat);

        void setRotation(const glm::mat4& matrix);

        void setTransform(const glm::mat4& matrix);

        const glm::vec3& getPosition() const;

        const glm::vec3& getScale() const;

        const glm::quat& getRotation() const;

        glm::mat4 getTransform() const;

    private:
        glm::vec3 mPosition;
        glm::quat mRotation;
        glm::vec3 mScale;
    };
} // Renderer

#endif //TRANSFORMATION_H
