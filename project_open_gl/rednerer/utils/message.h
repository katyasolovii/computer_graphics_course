//
// Created by Andrii Krenevych on 16.06.2025.
//

#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>

#include "glm/vec4.hpp"


std::string messageProperty(const std::string& propertyName, const glm::vec4& color);

std::string messageProperty(const std::string& propertyName, const float value) ;

std::string messageProperty(const std::string& propertyName, const int value);


#endif //MESSAGE_H
