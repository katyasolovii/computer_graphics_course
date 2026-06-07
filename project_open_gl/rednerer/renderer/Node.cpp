//
// Created by Andrii Krenevych on 10.06.2025.
//

#include "Node.h"

#include <functional>
#include <utility>

#include "Constants.h"
#include "Logger.h"
#include "Transformation.h"

namespace Renderer {
    Node::Node()
        : Node(DEFAULT_NODE_NAME) {
    }

    Node::Node(std::string name)
        : mName(std::move(name)),
          mTransform(std::make_shared<Transformation>()),
          mWorldTransform() {
        Logger::log().d("Create node: " + mName);
    }

    void Node::draw(const std::shared_ptr<Camera>& camera,
                    const std::vector<std::shared_ptr<NodeLight>>& lights) {
    }

    void Node::render(const std::shared_ptr<Camera>& camera,
                      const std::vector<std::shared_ptr<NodeLight>>& lights) {
        draw(camera, lights);
        for (const auto& child: mChildren) {
            child->render(camera, lights);
        }
    }

    void Node::setName(const std::string& name) {
        mName = name;
    }

    const std::string& Node::getName() const {
        return mName;
    }

    void Node::setPosition(const glm::vec3& position) const {
        mTransform->setPosition(position);
    }

    void Node::setScale(float scale) const {
        setScale(glm::vec3(scale, scale, scale));
    }

    void Node::setScale(const glm::vec3& scale) const {
        mTransform->setScale(scale);
    }

    void Node::setRotationEuler(glm::vec3 eulerDegrees) const {
        mTransform->setRotationEuler(eulerDegrees);
    }

    void Node::setRotation(const glm::quat& quat) const {
        mTransform->setRotation(quat);
    }

    void Node::setRotation(const glm::mat4& matrix) const {
        mTransform->setRotation(matrix);
    }

    void Node::setTransformation(const glm::mat4& matrix) const {
        mTransform->setTransform(matrix);
    }

    const glm::vec3& Node::getPosition() const {
        return mTransform->getPosition();
    }

    const glm::vec3& Node::getScale() const {
        return mTransform->getScale();
    }

    const glm::quat& Node::getRotation() const {
        return mTransform->getRotation();
    }

    glm::mat4 Node::getTransformation() const {
        return mTransform->getTransform();
    }

    void Node::addNode(std::shared_ptr<Node> child) {
        if (!child) return;

        if (const auto parent = child->getParent()) {
            if (parent.get() == this) {
                return;
            }

            parent->removeNode(child);
        }

        child->mParent = shared_from_this(); // встановлюємо parent
        mChildren.push_back(std::move(child));
    }

    void Node::removeNode(const std::shared_ptr<Node>& child) {
        auto it = std::find(mChildren.begin(), mChildren.end(), child);
        if (it != mChildren.end()) {
            mChildren.erase(it);
            child->mParent.reset(); // очищає weak_ptr
        }
    }

    std::shared_ptr<Node> Node::getParent() const {
        return mParent.lock(); // перетворюємо weak_ptr у shared_ptr
    }

    const std::vector<std::shared_ptr<Node> >& Node::getChildren() const {
        return mChildren;
    }

    void Node::updateTransform(const glm::mat4& parentTransform) {
        mWorldTransform = parentTransform * mTransform->getTransform();

        for (auto& child: mChildren) {
            child->updateTransform(mWorldTransform);
        }
    }

    std::shared_ptr<Node> Node::findNodeByName(const std::string& name) {
        if (getName() == name) {
            return shared_from_this();
        }

        for (const auto& child: mChildren) {
            if (auto result = child->findNodeByName(name)) return result;
        }

        return nullptr; // Не знайдено
    }

    std::shared_ptr<Node> Node::findNode(const std::function<bool(const Node&)>& predicate) {
        if (predicate(*this)) {
            return shared_from_this();
        }

        for (const auto& child: mChildren) {
            if (auto result = child->findNode(predicate)) return result;
        }

        return nullptr; // Не знайдено
    }

    void Node::accept(NodeVisitor& visitor) {
        for (const auto& child: mChildren) {
            child->accept(visitor);
        }
    }
} // Renderer
