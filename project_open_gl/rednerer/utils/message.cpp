//
// Created by Andrii Krenevych on 16.06.2025.
//

#include "message.h"

#include "glm/vec4.hpp"


std::string messageProperty(const std::string& propertyName, const glm::vec4& color) {
    std::string res = std::string("Found property: ") + propertyName;
    res += " : (" + std::to_string(color.r) + ", " +
            std::to_string(color.g) + ", " +
            std::to_string(color.b) + ", " +
            std::to_string(color.a) + ")";

    return res;
}

std::string messageProperty(const std::string& propertyName, const float value) {
    std::string res = std::string("Found property: ") + propertyName;
    res += " : (" + std::to_string(value) + ")";

    return res;
}

std::string messageProperty(const std::string& propertyName, const int value) {
    std::string res = std::string("Found property: ") + propertyName;
    res += " : (" + std::to_string(value) + ")";

    return res;
}
