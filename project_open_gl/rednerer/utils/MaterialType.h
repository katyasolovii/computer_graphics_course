//
// Created by Andrii Krenevych on 16.06.2025.
//

#ifndef MATERIAL_TYPE_H
#define MATERIAL_TYPE_H

#include <iostream>

enum class MaterialType {
    Unknown,             // Матеріал не ідентифіковано
    Unlit,               // Без освітлення, лише емісія або базовий колір
    Lambert,             // Простий дифузний матеріал
    Phong,               // Diffuse + Specular + Shininess
    PBR_MetalRoughness,  // PBR модель: металевість + шорсткість
    PBR_SpecGloss,       // PBR модель: specular + glossiness
    Glass,               // Прозорий, з індексом заломлення
    Toon,                // Cartoon / Cel shading
    Translucent          // Напівпрозорий (SSS або просто alpha blending)
};

inline std::string toString(MaterialType type) {
    switch (type) {
        case MaterialType::Unknown:            return "Unknown";
        case MaterialType::Unlit:              return "Unlit";
        case MaterialType::Lambert:            return "Lambert";
        case MaterialType::Phong:              return "Phong";
        case MaterialType::PBR_MetalRoughness: return "PBR_MetalRoughness";
        case MaterialType::PBR_SpecGloss:      return "PBR_SpecGloss";
        case MaterialType::Glass:              return "Glass";
        case MaterialType::Toon:               return "Toon";
        case MaterialType::Translucent:        return "Translucent";
        default:                               return "Invalid";
    }
}


inline std::ostream& operator<<(std::ostream& os, MaterialType type) {
    return os << toString(type);
}

#endif //MATERIAL_TYPE_H

