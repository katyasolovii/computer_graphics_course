#include <memory>

#include "SceneManagerImpl.h"

// unique_ptr - вказівник з ексклюзивним володінням об'єктом (пам'ять очищується автоматично, коли об'єкт виходить із зони видимості)
std::unique_ptr<Renderer::SceneManager> createSceneManager() {
    return std::make_unique<SceneManagerImpl>();
}