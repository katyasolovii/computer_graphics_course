//
// Created by Andrii Krenevych on 11.06.2025.
//

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <glm/glm.hpp>
#include "assimp/matrix4x4.h"


glm::mat4 convert(const aiMatrix4x4& aiMat);

void printMatrix(const glm::mat4& mat);

bool matricesEqualEpsilon(const glm::mat4& a, const glm::mat4& b, float epsilon = 1e-5f);

#endif //MATH_UTILS_H
