//
// Created by Andrii Krenevych on 21.04.2025.
//

#include <utility>

#include "Logger.h"
#include "Program.h"
#include "Mesh.h"

#include <algorithm>

namespace Renderer {
    Mesh::Mesh()
        : VAO(0),
          invalidAttribute(true),
          mVertBufferId(0),
          invalidVertices(true),
          mIndexBufferId(0),
          invalidIndices(true) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &mVertBufferId);
    }

    Mesh::~Mesh() {
        glDeleteBuffers(1, &mVertBufferId);
        if (mIndexBufferId != 0)
            glDeleteBuffers(1, &mIndexBufferId);
        glDeleteVertexArrays(1, &VAO);
    }

    void Mesh::setVertices(const std::vector<float>& vertices) {
        mVertices = vertices;
        invalidVertices = true;
    }

    void Mesh::setVertices(std::vector<float>&& vertices) {
        mVertices = std::move(vertices);
        invalidVertices = true;
    }

    void Mesh::setIndices(const std::vector<unsigned int>& indices) {
        mIndices = indices;
        invalidIndices = true;
    }

    void Mesh::setIndices(std::vector<unsigned int>&& indices) {
        mIndices = std::move(indices);
        invalidIndices = true;
    }

    void Mesh::addAttribute(std::string name, const int size) {
        const auto attrib = Attribute{
            .name = std::move(name),
            .size = size,
        };
        mAttributes.push_back(attrib);

        invalidAttribute = true;
    }

    void Mesh::update(const std::shared_ptr<Program>& program) {
        updateVertexBuffer();
        updateIndexBuffer();
        updateAttributes(program);
    }

    void Mesh::draw() const {
        glBindVertexArray(VAO);

        if (mIndexBufferId != 0) {
            Logger::log()
                    .d(std::string("Mesh::draw (index, glDrawElements): VAO: " + std::to_string(VAO)));
            // індексна модель, бо згенерований індексний буфер
            glDrawElements(GL_TRIANGLES, getVertexCount(), GL_UNSIGNED_INT, nullptr);
        } else {
            Logger::log()
                    .d(std::string("Mesh::draw (vertex, glDrawArrays): VAO: " + std::to_string(VAO)));
            glDrawArrays(GL_TRIANGLES, 0, getVertexCount());
        }
        glBindVertexArray(0);
    }

    int Mesh::getVertexCount() const {
        return mIndexBufferId != 0
                   ? static_cast<int>(mIndices.size())
                   : static_cast<int>(mVertices.size() / getStride());
    }

    int Mesh::getIndexCount() const {
        return mIndexBufferId != 0 ? static_cast<int>(mIndices.size()) : 0;
    }

    int Mesh::getStride() const {
        auto stride = 0;
        for (const auto& attrib: mAttributes) {
            stride += attrib.size;
        }
        return stride;
    }

    bool Mesh::hasAttribute(const char* attributeName) const {
        const auto it = std::find_if(
            mAttributes.begin(), mAttributes.end(),
            [&attributeName](const Attribute& attr) {
                return attr.name == attributeName;
            });

        return it != mAttributes.end();
    }

    void Mesh::invalidate() {
        Logger::log().d("Mesh::invalidate");
        invalidAttribute = true;
    }

    void Mesh::updateVertexBuffer() {
        if (invalidVertices) {

            Logger::log().d("Mesh::updateVertexBuffer");

            glBindBuffer(GL_ARRAY_BUFFER, mVertBufferId);
            glBufferData(GL_ARRAY_BUFFER,
                         static_cast<int>(mVertices.size() * sizeof(float)),
                         mVertices.data(),
                         GL_STATIC_DRAW
            );
            invalidVertices = false;
        }
    }

    void Mesh::updateIndexBuffer() {
        if (invalidIndices) {

            Logger::log().d("Mesh::updateIndexBuffer");

            if (mIndexBufferId == 0) {
                glGenBuffers(1, &mIndexBufferId);
            }
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         static_cast<int>(sizeof(unsigned int) * mIndices.size()),
                         mIndices.data(),
                         GL_STATIC_DRAW
            );
            invalidIndices = false;
        }
    }

    void Mesh::updateAttributes(const std::shared_ptr<Program>& program) {
        if (invalidAttribute) {

            Logger::log().d(std::string("Mesh::updateAttributes: VAO: ") + std::to_string(VAO));

            invalidAttribute = false;
            const auto stride = getStride();

            int offset = 0;
            for (auto& attrib: mAttributes) {
                attrib.location = glGetAttribLocation(program->getShaderProgram(), attrib.name.c_str());
                attrib.stride = stride;
                attrib.offset = offset;
                offset += attrib.size;
            }

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, mVertBufferId);
            if (mIndexBufferId != 0)
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);

            for (const auto& attrib: mAttributes) {
                glEnableVertexAttribArray(attrib.location);
                glVertexAttribPointer(
                    attrib.location, // location
                    attrib.size, // size
                    GL_FLOAT, // тип даних
                    GL_FALSE, // не нормалізувати
                    static_cast<int>(sizeof(float)) * attrib.stride, // stride
                    (void*) (sizeof(float) * attrib.offset) // offset
                );
            }

            glBindVertexArray(0);
        }
    }
} // Renderer
