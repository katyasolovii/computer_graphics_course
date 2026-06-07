//
// Created by Andrii Krenevych on 12.05.2026.
//

#ifndef NODE_LIGHT_H
#define NODE_LIGHT_H

#include <glm/glm.hpp>

#include "Node.h"

namespace Renderer {
    class Material;

    // Абстрактний базовий клас для всіх типів джерел світла
    class NodeLight : public Node {
    public:
        NodeLight();

        explicit NodeLight(std::string name);

        ~NodeLight() override = default;

        // Колір освітлення
        void setColor(const glm::vec3& color);
        const glm::vec3& getColor() const;

        // Інтенсивність
        void setIntensity(float intensity);
        float getIntensity() const;

        // Застосовує параметри цього джерела світла до матеріалу (реалізується у підкласах)
        virtual void applyToMaterial(Material& material) const = 0;

    protected:
        glm::vec3 mColor{1.0f, 1.0f, 1.0f};
        float mIntensity = 1.0f;
    };

} // Renderer

#endif // NODE_LIGHT_H
