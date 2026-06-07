//
// Created by Andrii Krenevych on 12.05.2026.
//

#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/glm.hpp>

#include "NodeLight.h"

namespace Renderer {

    // Спрямоване джерело світла (сонце) — рівномірно освітлює всю сцену з заданого напрямку
    class DirectionalLight : public NodeLight {
    public:
        DirectionalLight();

        explicit DirectionalLight(std::string name);

        ~DirectionalLight() override = default;

        void setDirection(const glm::vec3& direction);
        const glm::vec3& getDirection() const;

        void applyToMaterial(Material& material) const override;

        void accept(NodeVisitor& visitor) override;

    private:
        glm::vec3 mDirection{0.0f, -1.0f, 0.0f};
    };

} // Renderer

#endif // DIRECTIONAL_LIGHT_H
