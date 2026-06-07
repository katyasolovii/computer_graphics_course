#version 330 core

in vec3 vFragPos;   // Позиція поточного пікселя у 3D просторі
in vec3 vNormal;    // Нормаль до поверхні (напрям "вгору" від сфери)
in vec2 vTexCoord;  // Координати текстури (не використовуються)

out vec4 FragColor; // Кінцевий колір пікселя (R, G, B, A)


uniform vec3 uViewPos; // Позиція камери у 3D просторі

// Властивості матеріалу
uniform vec3  uColorAmbient;   // Фоновий колір матеріалу
uniform vec3  uColorDiffuse;   // Основний колір
uniform vec3  uColorSpecular;  // Колір відблиску
uniform float uShininess;      // Глянсовість (світлність)

// Параметри точкового джерела світла
uniform vec3  uPointLightPos;       // Позиція лампочки у сцені
uniform vec3  uPointLightColor;     // Колір лампочки
uniform float uPointLightIntensity; // Яскравість
uniform float uPointLightConstant;  // Коефіцієнт затухання: стала частина
uniform float uPointLightLinear;    // Коефіцієнт затухання: лінійна частина
uniform float uPointLightQuadratic; // Коефіцієнт затухання: квадратична частина

void main() {
    vec3 normal = normalize(vNormal);            // Нормалізуємо нормаль
    vec3 viewDir = normalize(uViewPos - vFragPos); // Напрям від пікселя до камери

    vec3  toLight = uPointLightPos - vFragPos; // Вектор від пікселя до світла
    vec3  lightDir = normalize(toLight);        // Нормалізований напрям до світла
    float dist = length(toLight);           // Відстань до джерела світла

    //  Ambient (фоновий)
    vec3 ambient = uPointLightColor * uColorAmbient;

    // Diffuse (розсіяний)
    float diff = max(dot(normal, lightDir), 0.0); // 0 якщо світло ззаду
    vec3 diffuse = diff * uColorDiffuse * uPointLightColor;

    // Specular (відблиск) - метод Blinn-Phong
    // Halfway-вектор — між напрямком до світла і до камери
    vec3  halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), max(uShininess, 1.0));
    vec3  specular = spec * uColorSpecular * uPointLightColor;

    //  Затухання (attenuation)
    float attenuation = 1.0 / (uPointLightConstant +
                                uPointLightLinear    * dist +
                                uPointLightQuadratic * dist * dist);

    // Застосовуємо інтенсивність і затухання до всіх компонентів
    ambient *= uPointLightIntensity * attenuation;
    diffuse *= uPointLightIntensity * attenuation;
    specular *= uPointLightIntensity * attenuation;

    // Глобальний ambient
    // Постійне базове підсвічування сцени (як розсіяне денне світло)
    vec3 globalAmbient = vec3(0.24) * uColorDiffuse;

    vec3 finalColor = ambient + diffuse + specular + globalAmbient;
    FragColor = vec4(finalColor, 1.0); // alpha = 1.0 (непрозорий)
}