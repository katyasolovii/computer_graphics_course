//
// Created by Andrii Krenevych on 12.05.2026.
//

#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <glm/glm.hpp>

#include "NodeLight.h"

namespace Renderer {

    // Прожекторне джерело світла — конус світла з позиції ноди в заданому напрямку
    class SpotLight : public NodeLight {
    public:
        SpotLight();

        explicit SpotLight(std::string name);

        ~SpotLight() override = default;

        void setDirection(const glm::vec3& direction);
        const glm::vec3& getDirection() const;

        // Кут внутрішнього конуса (у градусах) — повна яскравість
        void setCutOffAngle(float degrees);
        float getCutOffAngle() const;

        // Кут зовнішнього конуса (у градусах) — плавне загасання
        void setOuterCutOffAngle(float degrees);
        float getOuterCutOffAngle() const;

        void applyToMaterial(Material& material) const override;

        void accept(NodeVisitor& visitor) override;

    private:
        glm::vec3 mDirection{0.0f, -1.0f, 0.0f};
        float mCutOffAngle      = 12.5f;
        float mOuterCutOffAngle = 17.5f;
    };

} // Renderer

#endif // SPOT_LIGHT_H
