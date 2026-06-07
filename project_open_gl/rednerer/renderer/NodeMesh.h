//
// Created by Andrii Krenevych on 21.04.2025.
//

#ifndef NODE_MESH_H
#define NODE_MESH_H
#include <memory>


#include "Node.h"

namespace Renderer {

    class Camera;
    class Material;
    class Mesh;

    class NodeMesh : public Node {
    public:
        NodeMesh();
        explicit NodeMesh(std::string name);

        ~NodeMesh() override;

        void draw(const std::shared_ptr<Camera>& camera,
                  const std::vector<std::shared_ptr<NodeLight>>& lights) override;

        void setMesh(std::unique_ptr<Mesh> &&mesh);
        void setMaterial(const std::shared_ptr<Material>& material);

        void setDrawOrder(int order);
        int getDrawOrder() const;

        void setVisible(bool visible);
        bool isVisible() const;

        void setRenderMask(long mask);
        long getRenderMask() const;

        std::shared_ptr<Material>& getMaterial();

        unsigned int getVertexCount() const;

        unsigned int getIndexCount() const;

        void accept(NodeVisitor& visitor) override;

    private:

        int mDrawOrder = 0;
        bool mVisible = true;
        long mRenderMask = -1;  // бітова маска видимості

        std::unique_ptr<Mesh> mMesh;
        std::shared_ptr<Material> mMaterial;

    };
} // Renderer

#endif // NODE_MESH_H
