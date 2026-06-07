//
// Created by Andrii Krenevych on 21.04.2025.
//

#ifndef MESH_H
#define MESH_H
#include <memory>
#include <vector>
#include <string>

namespace Renderer {
    class Program;

    struct Attribute {
        std::string name;
        int size;
        int offset;
        int stride;
        unsigned int location;
    };

    class Mesh {
    public:
        Mesh();

        virtual ~Mesh();

        void setVertices(const std::vector<float>& vertices);

        void setVertices(std::vector<float>&& vertices);

        void setIndices(const std::vector<unsigned int>& indices);

        void setIndices(std::vector<unsigned int>&& indices);

        void addAttribute(std::string name, int size);

        void update(const std::shared_ptr<Program>& program);

        void draw() const;

        [[nodiscard]] int getVertexCount() const;

        [[nodiscard]] int getIndexCount() const;

        [[nodiscard]] int getStride() const;

        bool hasAttribute(const char* attributeName) const;

        void invalidate();

    private:
        void updateVertexBuffer();
        void updateIndexBuffer();
        void updateAttributes(const std::shared_ptr<Program>& program);

        unsigned int VAO; // vertex array object
        bool invalidAttribute;

        unsigned int mVertBufferId; // data
        std::vector<float> mVertices;
        bool invalidVertices;

        unsigned int mIndexBufferId; // data
        std::vector<unsigned int> mIndices;
        bool invalidIndices;

        std::vector<Attribute> mAttributes;  // TODO: consider to make map
    };
} // Renderer

#endif // MESH_H
