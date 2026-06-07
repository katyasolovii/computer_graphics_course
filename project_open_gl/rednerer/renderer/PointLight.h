//
// Created by Andrii Krenevych on 12.05.2026.
//

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "NodeLight.h"

namespace Renderer {

    // Точкове джерело світла — випромінює в усі сторони з позиції ноди, загасає з відстанню
    class PointLight : public NodeLight {
    public:
        PointLight();

        explicit PointLight(std::string name);

        ~PointLight() override = default;

        // Коефіцієнти загасання: constant + linear*d + quadratic*d^2
        void setAttenuation(float constant, float linear, float quadratic);
        float getConstant() const;
        float getLinear() const;
        float getQuadratic() const;

        void applyToMaterial(Material& material) const override;

        void accept(NodeVisitor& visitor) override;

    private:
        float mConstant  = 1.0f;
        float mLinear    = 0.09f;
        float mQuadratic = 0.032f;
    };

} // Renderer

#endif // POINT_LIGHT_H
