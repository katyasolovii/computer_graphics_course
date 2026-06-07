//
// Created by Andrii Krenevych on 21.04.2025.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include <map>
#include <memory>

#include "Program.h"
#include "Property.h"
#include "Texture.h"

namespace Renderer {
    class Material {
    public:
        explicit Material(std::string name);

        Material();

        virtual ~Material();

        void activateProgram() const;

        void bind() const;

        void unbind() const;

        void setProgram(const std::shared_ptr<Program>& program);
        [[nodiscard]] const std::shared_ptr<Program>& getProgram() const;

        void setTexture(const std::string& name, const std::shared_ptr<Texture>& texture);

        void setProperty(const std::shared_ptr<Property>& property);
        void setProperty(const std::string& name, const glm::mat4& value);
        void setProperty(const std::string& name, const glm::vec3& value);
        void setProperty(const std::string& name, const glm::vec4& value);
        void setProperty(const std::string& name, float value);
        void setProperty(const std::string& name, int value);

        [[nodiscard]] bool hasTexture(const std::string& str) const;

        [[nodiscard]] bool hasProperty(const std::string& str) const;

    private:
        std::string mName;

        std::shared_ptr<Program> mProgram;
        std::map<std::string, std::shared_ptr<Texture> > mTextures;
        std::map<std::string, std::shared_ptr<Property> > mProperties;
    };
} // Renderer

#endif //MATERIAL_H
