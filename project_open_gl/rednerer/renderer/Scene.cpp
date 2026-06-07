//
// Created by Andrii Krenevych on 21.04.2025.
//

#include "Camera.h"
#include "Node.h"
#include "NodeLight.h"
#include "Scene.h"
#include "Constants.h"
#include "Logger.h"

namespace Renderer {
    Scene::Scene()
        : mRootNode(std::make_shared<Node>(ROOT_NODE_NAME)) {

        mDefaultCamera = std::make_shared<Camera>(45.0f,
                                                       1.0,
                                                       0.1f,
                                                       1000.0f);
        addCamera(mDefaultCamera);
    }

    void Scene::addNode(std::shared_ptr<Node> model) {
        mRootNode->addNode(std::move(model));
    }

    void Scene::addCamera(std::shared_ptr<Camera> camera) {
        mCameras.push_back(std::move(camera));
    }

    void Scene::addLight(std::shared_ptr<NodeLight> light) {
        mLights.push_back(std::move(light));
    }

    void Scene::removeLight(const std::shared_ptr<NodeLight>& light) {
        auto it = std::find(mLights.begin(), mLights.end(), light);
        if (it != mLights.end()) {
            mLights.erase(it);
        }
    }

    std::shared_ptr<Camera>& Scene::getDefaultCamera() {
        return mDefaultCamera;
    }

    const std::vector<std::shared_ptr<NodeLight>>& Scene::getLights() const {
        return mLights;
    }

    void Scene::onViewportChange(int width, int height) const {
        for (const auto& camera: mCameras) {
            camera->onViewportChange(width, height);
        }
    }

    void Scene::render() const {
        Logger::log().d("Scene::start render");

        mRootNode->updateTransform(glm::mat4(1.0f));

        const auto cameras = getCamerasSortedByDrawOrder();
        for (const auto& camera: cameras) {
            camera->applyDepthTest();
            camera->applyClear();
            mRootNode->render(camera, mLights);
        }

        Logger::log().d("Scene::finish render");
    }

    std::shared_ptr<Node> Scene::findNodeByName(const std::string& name) const {
        return mRootNode->findNodeByName(name);
    }

    std::shared_ptr<Node> Scene::getRoot() const {
        return mRootNode;
    }

    std::vector<std::shared_ptr<Camera> > Scene::getCamerasSortedByDrawOrder() const {
        std::vector<std::shared_ptr<Camera> > sortedCameras = mCameras;
        std::sort(sortedCameras.begin(), sortedCameras.end(),
                  [](const std::shared_ptr<Camera>& a, const std::shared_ptr<Camera>& b) {
                      return a->getDrawOrder() < b->getDrawOrder();
                  });
        return sortedCameras;
    }
} // Renderer
