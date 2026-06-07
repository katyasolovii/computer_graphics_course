//
// Created by Andrii Krenevych on 21.04.2025.
//

#ifndef PROPERTY_H
#define PROPERTY_H
#include <string>

#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"

namespace Renderer {
    class Property {
    public:
        explicit Property(std::string name);

        virtual ~Property() = default;

        virtual void bind(unsigned int shaderProgram) = 0;

        [[nodiscard]] const std::string& getName() const;

    protected:
        [[nodiscard]] GLint location(unsigned int shaderProgram) const;

        std::string mName;
    };

    /*   PropertyInt   */
    class PropertyInt : public Property {
    public:
        PropertyInt(std::string name, int value);

        void bind(unsigned int shaderProgram) override;

    private:
        int mValue;
    };

    /*   PropertyFloat   */
    class PropertyFloat : public Property {
    public:
        PropertyFloat(std::string name, float value);

        void bind(unsigned int shaderProgram) override;

    private:
        float mValue;
    };

    /*   PropertyVec3   */
    class PropertyVec3 : public Property {
    public:
        PropertyVec3(std::string name, float x, float y, float z);

        PropertyVec3(std::string name, const glm::vec3& value);

        void bind(unsigned int shaderProgram) override;

    private:
        glm::vec3 mValue;
    };

    /*   PropertyVec4   */
    class PropertyVec4 : public Property {
    public:
        PropertyVec4(std::string name, float x, float y, float z, float w);

        PropertyVec4(std::string name, const glm::vec4& value);

        void bind(unsigned int shaderProgram) override;

    private:
        glm::vec4 mValue;
    };

    /*   PropertyMat4   */
    class PropertyMat4 : public Property {
    public:
        PropertyMat4(std::string name, const glm::mat4& value);

        void bind(unsigned int shaderProgram) override;

    private:
        glm::mat4 mValue;
    };
} // Renderer

#endif //PROPERTY_H
