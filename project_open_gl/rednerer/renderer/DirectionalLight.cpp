//
// Created by Andrii Krenevych on 12.05.2026.
//

#include "DirectionalLight.h"

#include "Constants.h"
#include "Material.h"
#include "NodeVisitor.h"

namespace Renderer {

    DirectionalLight::DirectionalLight()
        : NodeLight(DEFAULT_DIRECTIONAL_LIGHT_NAME) {
    }

    DirectionalLight::DirectionalLight(std::string name)
        : NodeLight(std::move(name)) {
    }

    void DirectionalLight::setDirection(const glm::vec3& direction) {
        mDirection = direction;
    }

    const glm::vec3& DirectionalLight::getDirection() const {
        return mDirection;
    }

    void DirectionalLight::applyToMaterial(Material& material) const {
        material.setProperty(UNIFORM_LIGHT_DIRECTION, mDirection);
        material.setProperty(UNIFORM_LIGHT_INTENSITY, mIntensity);
        material.setProperty(UNIFORM_LIGHT_COLOR, mColor);
    }

    void DirectionalLight::accept(NodeVisitor& visitor) {
        visitor.visit(*this);
        Node::accept(visitor);
    }

} // Renderer
