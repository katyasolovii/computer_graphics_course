//
// Created by Andrii Krenevych on 12.05.2026.
//

#include "PointLight.h"

#include "Constants.h"
#include "Material.h"
#include "NodeVisitor.h"

namespace Renderer {

    PointLight::PointLight()
        : NodeLight(DEFAULT_POINT_LIGHT_NAME) {
    }

    PointLight::PointLight(std::string name)
        : NodeLight(std::move(name)) {
    }

    void PointLight::setAttenuation(const float constant, const float linear, const float quadratic) {
        mConstant  = constant;
        mLinear    = linear;
        mQuadratic = quadratic;
    }

    float PointLight::getConstant() const  { return mConstant;  }
    float PointLight::getLinear() const    { return mLinear;    }
    float PointLight::getQuadratic() const { return mQuadratic; }

    void PointLight::applyToMaterial(Material& material) const {
        // Позиція береться з локального трансформа ноди
        material.setProperty(UNIFORM_POINT_LIGHT_POSITION,  getPosition());
        material.setProperty(UNIFORM_POINT_LIGHT_COLOR,     mColor);
        material.setProperty(UNIFORM_POINT_LIGHT_INTENSITY, mIntensity);
        material.setProperty(UNIFORM_POINT_LIGHT_CONSTANT,  mConstant);
        material.setProperty(UNIFORM_POINT_LIGHT_LINEAR,    mLinear);
        material.setProperty(UNIFORM_POINT_LIGHT_QUADRATIC, mQuadratic);
    }

    void PointLight::accept(NodeVisitor& visitor) {
        visitor.visit(*this);
        Node::accept(visitor);
    }

} // Renderer
