//
// Created by Andrii Krenevych on 12.05.2026.
//

#include "NodeLight.h"

#include "Constants.h"

namespace Renderer {

    NodeLight::NodeLight()
        : NodeLight(DEFAULT_LIGHT_NAME) {
    }

    NodeLight::NodeLight(std::string name)
        : Node(std::move(name)) {
    }

    void NodeLight::setColor(const glm::vec3& color) {
        mColor = color;
    }

    const glm::vec3& NodeLight::getColor() const {
        return mColor;
    }

    void NodeLight::setIntensity(const float intensity) {
        mIntensity = intensity;
    }

    float NodeLight::getIntensity() const {
        return mIntensity;
    }

} // Renderer
