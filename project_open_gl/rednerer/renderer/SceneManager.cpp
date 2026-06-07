//
// Created by Andrii Krenevych on 10.06.2025.
//

#include "SceneManager.h"

#include "InputDispatcher.h"
#include "Scene.h"
#include "Camera.h"

namespace Renderer {
    SceneManager::SceneManager() {
        InputDispatcher::getInstance().addKeyboardListener(this);
        InputDispatcher::getInstance().addMouseListener(this);

        scene = std::make_unique<Scene>();
    }

    SceneManager::~SceneManager() {
        InputDispatcher::getInstance().removeKeyboardListener(this);
        InputDispatcher::getInstance().removeMouseListener(this);
    }

    void SceneManager::onCreate() {
    }

    void SceneManager::onDestroy() {
    }

    void SceneManager::onViewportChange(int width, int height) {
        mWindowSize = glm::vec2(width, height);

        scene->onViewportChange(width, height);
    }

    void SceneManager::render() {
        onFrame(0);

        scene->render();
    }

    void SceneManager::onFrame(float delta) {
    }

    std::shared_ptr<Camera>& SceneManager::getDefaultCamera() {
        return scene->getDefaultCamera();
    }
} // namespace Renderer
