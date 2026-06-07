//
// Created by Andrii Krenevych on 11.06.2025.
//

#include "math_utils.h"

#include <iostream>

#include "glm/gtc/type_ptr.hpp"
#include <glm/glm.hpp>


glm::mat4 convert(const aiMatrix4x4& aiMat) {
    const glm::mat4 res = {
        aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
        aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
        aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
        aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
    };

    return res;
}

bool matricesEqualEpsilon(const glm::mat4& a, const glm::mat4& b, float epsilon) {
    for (int i = 0; i < 4; ++i) {         // колонки
        for (int j = 0; j < 4; ++j) {     // рядки
            if (std::abs(a[i][j] - b[i][j]) > epsilon)
                return false;
        }
    }
    return true;
}

void printMatrix(const glm::mat4& mat) {
    for (int row = 0; row < 4; ++row) {
        std::cout << "| ";
        for (int col = 0; col < 4; ++col) {
            std::cout << mat[col][row] << " ";
        }
        std::cout << "|\n";
    }
}
