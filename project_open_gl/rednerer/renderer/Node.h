//
// Created by Andrii Krenevych on 10.06.2025.
//

#ifndef NODE_H
#define NODE_H
#include <functional>
#include <memory>
#include <string>

#include <vector>

#include <glm/glm.hpp>

namespace Renderer {
    class Transformation;
    class Camera;
    class NodeLight;
    class NodeVisitor;

    class Node : public std::enable_shared_from_this<Node> {
    public:
        Node();

        explicit Node(std::string name);

        virtual ~Node() = default;

        virtual void draw(const std::shared_ptr<Camera>& camera,
                          const std::vector<std::shared_ptr<NodeLight>>& lights);

        void render(const std::shared_ptr<Camera>& camera,
                    const std::vector<std::shared_ptr<NodeLight>>& lights);

        void setName(const std::string& name);

        const std::string& getName() const;

        void setTransformation(const glm::mat4& matrix) const;

        glm::mat4 getTransformation() const;

        void setPosition(const glm::vec3& position) const;

        void setScale(float scale) const;
        void setScale(const glm::vec3& scale) const;

        void setRotationEuler(glm::vec3 eulerDegrees) const;

        void setRotation(const glm::quat& quat) const;

        void setRotation(const glm::mat4& matrix) const;

        const glm::vec3& getPosition() const;

        const glm::vec3& getScale() const;

        const glm::quat& getRotation() const;

        void addNode(std::shared_ptr<Node> child);

        void removeNode(const std::shared_ptr<Node>& child);

        std::shared_ptr<Node> getParent() const;

        const std::vector<std::shared_ptr<Node> >& getChildren() const;

        void updateTransform(const glm::mat4& parentTransform);

        std::shared_ptr<Node> findNodeByName(const std::string& name);

        std::shared_ptr<Node> findNode(const std::function<bool(const Node&)>& predicate);

        virtual void accept(NodeVisitor& visitor);

    protected:
        std::string mName;
        std::shared_ptr<Transformation> mTransform;
        glm::mat4 mWorldTransform;

        std::weak_ptr<Node> mParent;
        std::vector<std::shared_ptr<Node> > mChildren;
    };
} // Renderer

#endif //NODE_H
