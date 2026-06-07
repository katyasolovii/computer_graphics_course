//
// Created by Andrii Krenevych on 10.06.2025.
//

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <memory>

#include "IKeyboardListener.h"
#include "IMouseListener.h"
#include "glm/vec2.hpp"

namespace Renderer {

    class Scene;
    class Camera;

    class SceneManager : public IKeyboardListener, IMouseListener {
    public:
        SceneManager();

        ~SceneManager() override;

        virtual void onCreate();

        virtual void onDestroy();

        void onViewportChange(int width, int height);

        void render();

        virtual void onFrame(float delta);

        std::shared_ptr<Camera>& getDefaultCamera();

    protected:
        std::unique_ptr<Scene> scene;

        glm::vec2 mWindowSize {1, 1};

    private:


    };
} // namespace Renderer

#endif //SCENEMANAGER_H
