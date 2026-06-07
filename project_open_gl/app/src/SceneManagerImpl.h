// Якщо цей файл ще не підключався — підключити.
#ifndef SCENE_MANAGER_IMPL_H
#define SCENE_MANAGER_IMPL_H

#include <memory>
#include <string>
#include <vector>

#include "SceneManager.h"   // інтерфейс який реалізуємо
#include "glm/vec2.hpp"     // 2D вектор (x, y)
#include "glm/vec3.hpp"     // 3D вектор (x, y, z)

// Попереднє оголошення класів — компілятор знає що вони існують,
// повний опис підтягнеться у .cpp.
namespace Renderer {
class Node;       // вузол дерева сцени
class NodeMesh;   // вузол з 3D геометрією
class PointLight; // точкове джерело світла
}

// Кожна кнопка — унікальний біт, щоб зберігати кілька натиснутих одночасно в одній змінній
constexpr long MOUSE_BUTTON_LEFT   = 1;      // 001
constexpr long MOUSE_BUTTON_RIGHT  = 1 << 1; // 010
constexpr long MOUSE_BUTTON_MIDDLE = 1 << 2; // 100

// Реалізація інтерфейсу SceneManager — вся логіка сцени
class SceneManagerImpl : public Renderer::SceneManager {
public:
    void onCreate() override;           // ініціалізація при старті
    void onFrame(float delta) override; // кожен кадр, delta — час між кадрами

    void onKeyUp(int key, int mods) override;     // клавішу відпустили
    void onKeyDown(int key, int mods) override;   // клавішу натиснули
    void onKeyRepeat(int key, int mods) override; // клавіша утримується

    void onMove(double xPos, double yPos) override;                              // курсор перемістився
    void onButtonDown(int button, double xPos, double yPos, int mods) override; // кнопку натиснули
    void onButtonUp(int button, double xPos, double yPos, int mods) override;   // кнопку відпустили
    void onScroll(double xOffset, double yOffset) override;                     // колесо миші

private:
    // Завантажує 3D модель з файлу, повертає корінь дерева вузлів.
    std::shared_ptr<Renderer::Node> loadModelNode(const std::string& path) const;

    void collectMeshNodes(const std::shared_ptr<Renderer::Node>& node); // збирає всі меші моделі у mMeshNodes
    void applyMaterialPreset(int presetIndex);                          // застосовує матеріал до всіх мешів
    void rotateModel();                                                 // обертає модель за рухом миші

    // Перетворює 2D позицію миші на 3D точку на уявній сфері — для плавного arcball-обертання
    glm::vec3 projectToArcball(glm::vec2 pos) const;

    long mMousePressed = 0;    // які кнопки зараз затиснуті
    glm::vec2 mCurrentMouse{}; // поточна позиція курсора
    glm::vec2 mLastMouse{};    // позиція курсора на попередньому кадрі

    std::shared_ptr<Renderer::Node> mModelNode;                  // корінь завантаженої моделі
    std::vector<std::shared_ptr<Renderer::NodeMesh>> mMeshNodes; // всі меші моделі
    std::shared_ptr<Renderer::PointLight> mPointLight;           // точкове світло у сцені
    int mCurrentMaterialIndex = 0;                               // індекс активного матеріалу
};

#endif