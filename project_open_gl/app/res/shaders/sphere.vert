#version 330 core

// Вхідні дані приходять з VAO/VBO, які Assimp заповнив з OBJ файлу
// location=0 → Position з "v" в OBJ
layout(location = 0) in vec3 aPosition;

// location=1 → Normal з "vn" в OBJ
layout(location = 1) in vec3 aNormal;

// location=2 → TexCoord з "vt" в OBJ
layout(location = 2) in vec2 aTexture;

// Матриці трансформації
uniform mat4 uModel;       // позиція/обертання/масштаб сфери у світі
uniform mat4 uView;        // матриця камери (де і куди дивиться)
uniform mat4 uProjection;  // перспективна проєкція

// Вихідні дані для фрагментного шейдера
out vec3 vFragPos;   // позиція вершини у world space (для освітлення)
out vec3 vNormal;    // трансформована нормаль (для освітлення)
out vec2 vTexCoord;  // UV координати (адреса пікселя на текстурі)

void main() {
    // Позиція вершини у world space
    // Потрібна фрагментному шейдеру для розрахунку відстані до світла
    vFragPos = vec3(uModel * vec4(aPosition, 1.0));

    // Коректна трансформація нормалі
    // Звичайне множення на uModel ламає нормаль при масштабуванні
    // transpose(inverse(uModel)) виправляє це спотворення
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;

    // Передаємо UV координати без змін у фрагментний шейдер
    vTexCoord = aTexture;

    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}