//
// Created by Andrii Krenevych on 21.04.2025.
//

#include "NodeMesh.h"

#include <utility>

#include "Camera.h"
#include "Constants.h"
#include "Logger.h"
#include "NodeLight.h"
#include "NodeVisitor.h"
#include "Transformation.h"

#include "Mesh.h"
#include "Material.h"

namespace Renderer {
    NodeMesh::NodeMesh() = default;

    NodeMesh::NodeMesh(std::string name)
        : Node(std::move(name)) {
    }

    NodeMesh::~NodeMesh() = default;

    void NodeMesh::draw(const std::shared_ptr<Camera>& camera,
                        const std::vector<std::shared_ptr<NodeLight>>& lights) {
        if (!mVisible) {
            return;
        }

        if ((camera->getRenderMask() & mRenderMask) == 0) {
            return;
        }

        Logger::log().d("NodeModel::draw start: " + mName);

        mMaterial->activateProgram();
        mMesh->update(mMaterial->getProgram());

        ///// Transformation ///////
        const glm::mat4 viewMatr = camera->getViewMatrix();
        const glm::mat4 projectionMatr = camera->getProjectionMatrix();
        const glm::mat4 MVP = projectionMatr * viewMatr * mWorldTransform;

        mMaterial->setProperty(UNIFORM_MODEL, mWorldTransform);
        mMaterial->setProperty(UNIFORM_VIEW, viewMatr);
        mMaterial->setProperty(UNIFORM_PROJECTION, projectionMatr);
        mMaterial->setProperty(UNIFORM_MVP, MVP);

        ///// Camera ///////
        const glm::vec3 viewPos = glm::vec3(camera->getTransformation()[3]);
        mMaterial->setProperty(UNIFORM_VIEW_POSITION, viewPos);

        ///// Lighting ///////
        for (const auto& light : lights) {
            light->applyToMaterial(*mMaterial);
        }

        mMaterial->bind();

        mMesh->draw();

        mMaterial->unbind();

        Logger::log().d("NodeModel::draw finish: " + mName);
    }

    void NodeMesh::setMesh(std::unique_ptr<Mesh>&& mesh) {
        mMesh = std::move(mesh);
        mMesh->invalidate();
    }

    void NodeMesh::setMaterial(const std::shared_ptr<Material>& material) {
        mMaterial = material;
        if (mMesh != nullptr) {
            mMesh->invalidate();
        }
    }

    void NodeMesh::setDrawOrder(int order) {
        mDrawOrder = order;
    }

    int NodeMesh::getDrawOrder() const {
        return mDrawOrder;
    }

    void NodeMesh::setVisible(bool visible) {
        mVisible = visible;
    }

    bool NodeMesh::isVisible() const {
        return mVisible;
    }

    void NodeMesh::setRenderMask(const long mask) {
        mRenderMask = mask;
    }

    long NodeMesh::getRenderMask() const {
        return mRenderMask;
    }

    std::shared_ptr<Material>& NodeMesh::getMaterial() {
        return mMaterial;
    }

    unsigned int NodeMesh::getVertexCount() const {
        return mMesh->getVertexCount();
    }

    unsigned int NodeMesh::getIndexCount() const {
        return mMesh->getIndexCount();
    }

    void NodeMesh::accept(NodeVisitor& visitor) {
        visitor.visit(*this);
        Node::accept(visitor); // або прямо викликати для дітей
    }
} // Renderer
