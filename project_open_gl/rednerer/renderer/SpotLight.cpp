//
// Created by Andrii Krenevych on 12.05.2026.
//

#include "SpotLight.h"

#include "Constants.h"
#include "Material.h"
#include "NodeVisitor.h"

namespace Renderer {

    SpotLight::SpotLight()
        : NodeLight(DEFAULT_SPOT_LIGHT_NAME) {
    }

    SpotLight::SpotLight(std::string name)
        : NodeLight(std::move(name)) {
    }

    void SpotLight::setDirection(const glm::vec3& direction) {
        mDirection = direction;
    }

    const glm::vec3& SpotLight::getDirection() const {
        return mDirection;
    }

    void SpotLight::setCutOffAngle(const float degrees) {
        mCutOffAngle = degrees;
    }

    float SpotLight::getCutOffAngle() const {
        return mCutOffAngle;
    }

    void SpotLight::setOuterCutOffAngle(const float degrees) {
        mOuterCutOffAngle = degrees;
    }

    float SpotLight::getOuterCutOffAngle() const {
        return mOuterCutOffAngle;
    }

    void SpotLight::applyToMaterial(Material& material) const {
        // Позиція береться з локального трансформа ноди
        material.setProperty(UNIFORM_SPOT_LIGHT_POSITION,       getPosition());
        material.setProperty(UNIFORM_SPOT_LIGHT_DIRECTION,      mDirection);
        material.setProperty(UNIFORM_SPOT_LIGHT_COLOR,          mColor);
        material.setProperty(UNIFORM_SPOT_LIGHT_INTENSITY,      mIntensity);
        // Зберігаємо косинуси кутів — так зручніше порівнювати у шейдері
        material.setProperty(UNIFORM_SPOT_LIGHT_CUT_OFF,        glm::cos(glm::radians(mCutOffAngle)));
        material.setProperty(UNIFORM_SPOT_LIGHT_OUTER_CUT_OFF,  glm::cos(glm::radians(mOuterCutOffAngle)));
    }

    void SpotLight::accept(NodeVisitor& visitor) {
        visitor.visit(*this);
        Node::accept(visitor);
    }

} // Renderer
