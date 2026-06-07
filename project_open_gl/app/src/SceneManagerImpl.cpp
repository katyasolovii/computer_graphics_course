#include "SceneManagerImpl.h"

#include <algorithm>
#include <array>
#include <cfloat>
#include <cmath>
#include <string>
#include <utility>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Camera.h"
#include "Constants.h"
#include "Material.h"
#include "Mesh.h"
#include "Node.h"
#include "NodeMesh.h"
#include "PointLight.h"
#include "Program.h"
#include "Scene.h"
#include "GLFW/glfw3.h"
#include "glm/common.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include <glm/gtc/quaternion.hpp>

// Набір властивостей матеріалу — описує як виглядає поверхня під світлом
struct MaterialPreset {
    glm::vec3 ambient;   // колір у тіні
    glm::vec3 diffuse;   // основний колір
    glm::vec3 specular;  // колір відблиску
    float shininess;     // глянсовість, світлість
};

const std::string kModelPath = "res/models/sphere.obj"; // шлях до 3D моделі сфери

const glm::vec3 kCameraStartPos(0.0f, 2.0f, 5.0f); // початкова позиція камери
const glm::vec3 kCameraLookAt(0.0f, 0.0f, 0.0f);   // точка на яку дивиться камера

const float kLightRadius = 3.5f; // радіус кола по якому рухається світло
const float kLightHeight = 5.5f; // висота світла над сценою
const float kLightSpeed  = 0.8f; // швидкість обертання світла

// 6 матеріалів: золото, срібло, бронза, смарагд, хром, червоний пластик
//  (ambient/diffuse/specular/shininess)
const std::array<MaterialPreset, 6> kMaterialPresets{{
    {{0.24725f, 0.1995f,  0.0745f},  {0.75164f, 0.60648f, 0.22648f}, {0.62828f, 0.5558f,   0.36606f}, 51.2f},  // золото
    {{0.19225f, 0.19225f, 0.19225f}, {0.50754f, 0.50754f, 0.50754f}, {0.50827f, 0.50827f,  0.50827f}, 51.2f},  // срібло
    {{0.2125f,  0.1275f,  0.054f},   {0.714f,   0.4284f,  0.1814f},  {0.3935f,  0.2719f,   0.1667f},  25.6f},  // бронза
    {{0.0215f,  0.1745f,  0.0215f},  {0.07568f, 0.61424f, 0.07568f}, {0.633f,   0.727811f, 0.633f},   76.8f},  // смарагд
    {{0.25f,    0.25f,    0.25f},    {0.4f,     0.4f,     0.4f},     {0.774597f,0.774597f, 0.774597f},128.0f}, // хром
    {{0.0f,     0.0f,     0.0f},     {0.5f,     0.0f,     0.0f},     {0.7f,     0.7f,      0.7f},     32.0f},  // червоний пластик
}};

// Сірий колір за замовчуванням — до того як вибрано матеріал
const glm::vec3 kDefaultMaterialColor(0.8f, 0.8f, 0.8f);

void SceneManagerImpl::onCreate() {
    SceneManager::onCreate(); // запуск базового класу

    // Завантажуємо сферу і додаємо до сцени
    mModelNode = loadModelNode(kModelPath);
    if (mModelNode != nullptr) {
        scene->addNode(mModelNode);
        collectMeshNodes(mModelNode); // збираємо всі частини моделі щоб потім міняти матеріал
    }

    // Створюємо точкове світло (як лампочка) і налаштовуємо його
    mPointLight = std::make_shared<Renderer::PointLight>();
    mPointLight->setColor(glm::vec3(1.0f, 0.95f, 0.85f)); // тепло-біле світло
    mPointLight->setIntensity(3.0f);                       // яскравість
    mPointLight->setAttenuation(1.0f, 0.09f, 0.032f);     // як швидко слабне з відстанню
    mPointLight->setPosition(glm::vec3(kLightRadius, kLightHeight, 0.0f));
    scene->addLight(mPointLight);

    // Ставимо камеру на початкову позицію
    const auto camera = scene->getDefaultCamera();
    camera->setPosition(kCameraStartPos);
    camera->setLookAt(kCameraLookAt);

    applyMaterialPreset(0); // при старті — золото
}

void SceneManagerImpl::onFrame(float delta) {
    // Кожен кадр обертаємо світло по колу навколо сфери
    if (mPointLight != nullptr) {
        float angle = static_cast<float>(glfwGetTime()) * kLightSpeed; // кут залежить від часу
        mPointLight->setPosition(glm::vec3(
            kLightRadius * glm::cos(angle), // x — рух по колу
            kLightHeight,                   // y — висота не змінюється
            kLightRadius * glm::sin(angle)  // z — рух по колу
        ));
    }
}

void SceneManagerImpl::onKeyUp(int key, int mods) {
    // нічого не робимо при відпусканні клавіші
}

void SceneManagerImpl::onKeyDown(int key, int mods) {
    const auto camera = scene->getDefaultCamera();
    float speed = 0.5f; // крок руху камери

    switch (key) {
        case GLFW_KEY_W: camera->moveForward(speed);  break; // вперед
        case GLFW_KEY_S: camera->moveForward(-speed); break; // назад
        case GLFW_KEY_A: camera->moveRight(-speed);   break; // вліво
        case GLFW_KEY_D: camera->moveRight(speed);    break; // вправо
        case GLFW_KEY_Q: camera->moveUp(-speed);      break; // вниз
        case GLFW_KEY_E: camera->moveUp(speed);       break; // вгору

        // Клавіші 1–6 перемикають матеріал сфери
        case GLFW_KEY_1: applyMaterialPreset(0); break; // золото
        case GLFW_KEY_2: applyMaterialPreset(1); break; // срібло
        case GLFW_KEY_3: applyMaterialPreset(2); break; // бронза
        case GLFW_KEY_4: applyMaterialPreset(3); break; // смарагд
        case GLFW_KEY_5: applyMaterialPreset(4); break; // хром
        case GLFW_KEY_6: applyMaterialPreset(5); break; // червоний пластик
        default: break;
    }
}

void SceneManagerImpl::onKeyRepeat(int key, int mods) {
    onKeyDown(key, mods); // утримання клавіші = повторне натискання
}

void SceneManagerImpl::onMove(double xPos, double yPos) {
    if (mMousePressed == 0) return; // жодна кнопка не затиснута — нічого не робимо

    const auto camera = scene->getDefaultCamera();
    mCurrentMouse = glm::vec2(xPos, yPos);
    const auto delta = mCurrentMouse - mLastMouse; // наскільки перемістилась миша за кадр

    if (mMousePressed & MOUSE_BUTTON_LEFT) {
        rotateModel(); // ліва кнопка — крутимо сферу
    }
    if (mMousePressed & MOUSE_BUTTON_RIGHT) {
        float sensitivity = 0.1f;
        camera->rotate(-delta.x * sensitivity, delta.y * sensitivity); // права кнопка — крутимо камеру
    }
    if (mMousePressed & MOUSE_BUTTON_MIDDLE) {
        float sensitivity = 0.0125f;
        camera->moveLocal(glm::vec3(-delta.x, delta.y, 0.0f) * sensitivity); // середня кнопка — зсув камери
    }

    mLastMouse = mCurrentMouse; // запам'ятовуємо поточну позицію для наступного кадру
}

void SceneManagerImpl::onButtonDown(int button, double xPos, double yPos, int mods) {
    mLastMouse = mCurrentMouse = glm::vec2(xPos, yPos); // запам'ятовуємо де натиснули

    // Встановлюємо біт відповідної кнопки (позначаємо що вона затиснута)
    if (button == GLFW_MOUSE_BUTTON_LEFT)        mMousePressed |= MOUSE_BUTTON_LEFT;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)  mMousePressed |= MOUSE_BUTTON_RIGHT;
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE) mMousePressed |= MOUSE_BUTTON_MIDDLE;
}

void SceneManagerImpl::onButtonUp(int button, double xPos, double yPos, int mods) {
    // ^= скидає біт кнопки (позначаємо що вона відпущена)
    if (button == GLFW_MOUSE_BUTTON_LEFT)        mMousePressed ^= MOUSE_BUTTON_LEFT;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)  mMousePressed ^= MOUSE_BUTTON_RIGHT;
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE) mMousePressed ^= MOUSE_BUTTON_MIDDLE;
}

void SceneManagerImpl::onScroll(double xOffset, double yOffset) {
    const auto camera = scene->getDefaultCamera();
    camera->moveForward(static_cast<float>(yOffset) * 0.3f); // скрол — наближення/віддалення
}

std::shared_ptr<Renderer::Node> SceneManagerImpl::loadModelNode(const std::string& path) const {
    Assimp::Importer importer;

    // Читаємо .obj файл:
    // Triangulate — ділимо всі полігони (багатокутники) на трикутники
    // GenSmoothNormals — автоматично рахуємо згладжені нормалі
    // FlipUVs — перевертаємо координати текстури (OpenGL і більшість редакторів мають різний напрям Y)
    const aiScene* assimpScene = importer.ReadFile(
        path,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    // Якщо файл не знайдено або пошкоджено — повертаємо порожній вказівник
    if (assimpScene == nullptr ||
        (assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0 ||
        assimpScene->mRootNode == nullptr) {
        return nullptr;
    }

    // Шукаємо найменшу і найбільшу точку моделі —
    // потрібно щоб відцентрувати і підігнати розмір сфери під сцену
    glm::vec3 minBound( FLT_MAX,  FLT_MAX,  FLT_MAX);  // найменша точка
    glm::vec3 maxBound(-FLT_MAX, -FLT_MAX, -FLT_MAX);  // найбільша точка

    for (unsigned int meshIdx = 0; meshIdx < assimpScene->mNumMeshes; ++meshIdx) {
        const aiMesh* mesh = assimpScene->mMeshes[meshIdx];
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            glm::vec3 p(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            minBound = glm::min(minBound, p);
            maxBound = glm::max(maxBound, p);
        }
    }

    glm::vec3 center = (minBound + maxBound) * 0.5f; // геометричний центр моделі

    // Найбільший розмір по будь-якій осі (x, y або z)
    float maxExtent = std::max(maxBound.x - minBound.x,
                      std::max(maxBound.y - minBound.y,
                               maxBound.z - minBound.z));

    // Рахуємо на скільки масштабувати щоб сфера мала розмір ~3 одиниці
    float scaleFactor = maxExtent > 0.0f ? 3.0f / maxExtent : 1.0f;

    auto root    = std::make_shared<Renderer::Node>("LoadedModelRoot");
    auto program = std::make_shared<Renderer::Program>("res/shaders/sphere.vert", "res/shaders/sphere.frag");

    for (unsigned int meshIdx = 0; meshIdx < assimpScene->mNumMeshes; ++meshIdx) {
        const aiMesh* aiMesh = assimpScene->mMeshes[meshIdx];

        std::vector<float>        vertices; // дані вершин (позиція + нормаль + UV)
        std::vector<unsigned int> indices;  // індекси — які вершини утворюють трикутники

        for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i) {
            // Центруємо і масштабуємо позицію вершини
            glm::vec3 position(aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);
            position = (position - center) * scaleFactor;

            // Нормаль — напрям перпендикулярний поверхні, потрібен для освітлення
            glm::vec3 normal(0.0f, 1.0f, 0.0f); // дефолт — дивиться вгору
            if (aiMesh->HasNormals()) {
                normal = glm::vec3(aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z);
            }

            // UV — координати текстури (поки не використовуються, але зберігаємо)
            glm::vec2 uv(0.0f, 0.0f);
            if (aiMesh->HasTextureCoords(0)) {
                uv = glm::vec2(aiMesh->mTextureCoords[0][i].x, aiMesh->mTextureCoords[0][i].y);
            }

            // Записуємо всі дані вершини підряд: x y z  nx ny nz  u v
            vertices.push_back(position.x); vertices.push_back(position.y); vertices.push_back(position.z);
            vertices.push_back(normal.x);   vertices.push_back(normal.y);   vertices.push_back(normal.z);
            vertices.push_back(uv.x);       vertices.push_back(uv.y);
        }

        // Збираємо індекси — кожна грань (face) це один трикутник з 3 вершин
        for (unsigned int faceIdx = 0; faceIdx < aiMesh->mNumFaces; ++faceIdx) {
            const aiFace& face = aiMesh->mFaces[faceIdx];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }

        // Створюємо меш і кажемо GPU як читати дані вершин
        auto mesh = std::make_unique<Renderer::Mesh>();
        mesh->setVertices(std::move(vertices));
        mesh->setIndices(std::move(indices));
        mesh->addAttribute(ATTRIB_POSITION, 3); // перші 3 числа — позиція (x y z)
        mesh->addAttribute(ATTRIB_NORMAL,   3); // наступні 3 — нормаль (nx ny nz)
        mesh->addAttribute("aTexture",      2); // останні 2 — UV (u v)

        // Створюємо матеріал — прив'язуємо шейдер і встановлюємо початковий колір
        auto material = std::make_shared<Renderer::Material>("Material_" + std::to_string(meshIdx));
        material->setProgram(program);
        material->setProperty(MATERIAL_COLOR_AMBIENT,  kDefaultMaterialColor);
        material->setProperty(MATERIAL_COLOR_DIFFUSE,  kDefaultMaterialColor);
        material->setProperty(MATERIAL_COLOR_SPECULAR, glm::vec3(1.0f)); // білий відблиск
        material->setProperty(MATERIAL_SHININESS,      32.0f);

        // Об'єднуємо меш і матеріал у вузол (об'єкт) сцени
        auto nodeMesh = std::make_shared<Renderer::NodeMesh>("Mesh_" + std::to_string(meshIdx));
        nodeMesh->setMesh(std::move(mesh));
        nodeMesh->setMaterial(material);

        root->addNode(nodeMesh); // додаємо до кореневого вузла
    }

    return root;
}

void SceneManagerImpl::collectMeshNodes(const std::shared_ptr<Renderer::Node>& node) {
    if (node == nullptr) return;

    // Якщо цей вузол є мешем — додаємо до списку (для зміни матеріалу)
    // dynamic_pointer_cast — перевіряємо чи вузол є саме NodeMesh, а не просто Node
    if (const auto mesh = std::dynamic_pointer_cast<Renderer::NodeMesh>(node); mesh != nullptr) {
        mMeshNodes.push_back(mesh);
    }

    // Повторюємо для всіх дочірніх вузлів (обходимо все дерево)
    for (const auto& child : node->getChildren()) {
        collectMeshNodes(child);
    }
}

void SceneManagerImpl::applyMaterialPreset(int presetIndex) {
    // Перевіряємо що номер матеріалу існує у списку
    if (presetIndex < 0 || presetIndex >= static_cast<int>(kMaterialPresets.size())) return;

    mCurrentMaterialIndex = presetIndex;
    const MaterialPreset& preset = kMaterialPresets[presetIndex];

    // Застосовуємо вибраний матеріал до кожної частини моделі
    for (const auto& meshNode : mMeshNodes) {
        auto& material = meshNode->getMaterial();
        if (material == nullptr) continue;

        material->setProperty(MATERIAL_COLOR_AMBIENT,  preset.ambient);
        material->setProperty(MATERIAL_COLOR_DIFFUSE,  preset.diffuse);
        material->setProperty(MATERIAL_COLOR_SPECULAR, preset.specular);
        material->setProperty(MATERIAL_SHININESS,      preset.shininess);
    }
}

void SceneManagerImpl::rotateModel() {
    if (mModelNode == nullptr) return;

    // Arcball — спосіб обертання: уявляємо що сфера вписана у вікно,
    // і перетягуємо точку на ній мишею
    const glm::vec3 va = projectToArcball(mLastMouse);    // точка де була миша
    const glm::vec3 vb = projectToArcball(mCurrentMouse); // точка де миша зараз

    const glm::vec3 axis  = glm::cross(va, vb); // вісь обертання — перпендикуляр між двома точками
    const float angle     = glm::clamp(glm::dot(va, vb), -1.0f, 1.0f); // кут між ними

    if (glm::length(axis) > 0.0001f) { // якщо миша реально рухалась
        // Кватерніон (quaternion) — математичний спосіб зберігати обертання без "гімбального замку"
        const glm::quat delta_quat = glm::angleAxis(acosf(angle), glm::normalize(axis));
        mModelNode->setRotation(delta_quat * mModelNode->getRotation()); // додаємо нове обертання до поточного
    }
}

glm::vec3 SceneManagerImpl::projectToArcball(glm::vec2 pos) const {
    // Перетворюємо піксельні координати миші у діапазон [-1, 1]
    float nx   = (2.0f * pos.x - mWindowSize.x) / mWindowSize.x;
    float ny   = (mWindowSize.y - 2.0f * pos.y) / mWindowSize.y;
    float dist = nx * nx + ny * ny; // відстань від центру екрану

    if (dist <= 1.0f)
        return {nx, ny, sqrtf(1.0f - dist)}; // точка на сфері (є Z)

    return glm::normalize(glm::vec3(nx, ny, 0.0f)); // точка за межами сфери — нормалізуємо у площині
}