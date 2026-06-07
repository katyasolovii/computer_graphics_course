//
// Created by Andrii Krenevych on 11.06.2025.
//

#ifndef NODE_VISITOR_H
#define NODE_VISITOR_H


namespace Renderer {
    class Node;
    class NodeMesh;
    class DirectionalLight;
    class PointLight;
    class SpotLight;

    class NodeVisitor {
    public:
        virtual ~NodeVisitor() = default;

        virtual void visit(NodeMesh& nodeModel) = 0;

        // Необов'язкові методи для джерел світла — за замовчуванням нічого не роблять
        virtual void visit(DirectionalLight& light) {}
        virtual void visit(PointLight& light) {}
        virtual void visit(SpotLight& light) {}
    };
};


#endif // NODE_VISITOR_H
