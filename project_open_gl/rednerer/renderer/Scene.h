//
// Created by Andrii Krenevych on 21.04.2025.
//

#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include <vector>
#include <memory>

#include <functional>


namespace Renderer {
    class Camera;
    class Node;
    class NodeLight;

    class Scene {
    public:
        Scene();

        virtual ~Scene() = default;

        void addNode(std::shared_ptr<Node> model);

        void addCamera(std::shared_ptr<Camera> camera);

        void addLight(std::shared_ptr<NodeLight> light);

        void removeLight(const std::shared_ptr<NodeLight>& light);

        std::shared_ptr<Camera>& getDefaultCamera();

        const std::vector<std::shared_ptr<NodeLight>>& getLights() const;

        void onViewportChange(int width, int height) const;

        void render() const;

        [[nodiscard]] std::shared_ptr<Node> findNodeByName(const std::string& name) const;

        [[nodiscard]] std::shared_ptr<Node> getRoot() const;

    private:

        [[nodiscard]] std::vector<std::shared_ptr<Camera> > getCamerasSortedByDrawOrder() const;

        std::shared_ptr<Node> mRootNode;
        std::vector<std::shared_ptr<Camera> > mCameras;
        std::vector<std::shared_ptr<NodeLight>> mLights;

        std::shared_ptr<Camera> mDefaultCamera;
    };
} // Renderer

#endif //SCENE_BASE_H
