//
// Created by Andrii Krenevych on 16.06.2025.
//

#ifndef NODEMESHLOADER_H
#define NODEMESHLOADER_H
#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "glm/vec4.hpp"

#include "MaterialType.h"

struct aiMaterial;
struct aiScene;
struct aiMesh;

namespace Renderer {
    class Texture;
    class NodeMesh;
    class Material;
    class Mesh;
}

enum class  MaterialType;

struct Properties {
    std::map<std::string, std::shared_ptr<Renderer::Texture> > textures;
    std::map<std::string, glm::vec4> props4f;
    std::map<std::string, float> props1f;
    std::map<std::string, int> props1i;

    [[nodiscard]] bool hasTexture(const std::string& name) const{
        return textures.find(name) != textures.end();
    }

    [[nodiscard]] bool hasProperty(const std::string& name) const {
        return props4f.find(name) != props4f.end() || props1f.find(name) != props1f.end() || props1i.find(name) !=
               props1i.end();
    }
};

struct AttributeSpec {
    std::string name;
    int size;
};

struct Attributes {
    std::vector<AttributeSpec> attributes;

    bool hasAttribute(const std::string& name) {
        const auto it = std::find_if(
            attributes.begin(), attributes.end(),
            [&name](const AttributeSpec& attr) {
                return attr.name == name;
            });

        return it != attributes.end();
    }
};


class NodeMeshLoader {
public:
    NodeMeshLoader();

    std::shared_ptr<Renderer::NodeMesh> create(
        const aiScene* ai_scene,
        const aiMesh* ai_mesh,
        const std::string& name,
        const std::string& directory);

private:
    std::unique_ptr<Renderer::Mesh> createMesh(const aiMesh* mesh);

    std::vector<std::string> generatePropertyDefines();

    std::vector<std::string> generateAttributeDefines();

    //////// Material ////////////
    void parseMaterialProperties(
        const aiScene* scene,
        const aiMaterial* aiMat,
        const std::string& modelDirectory
    );

    std::shared_ptr<Renderer::Material> loadMaterial(
        const aiScene* scene,
        const aiMaterial* ai_material,
        const std::string& modelDirectory
    );

    std::shared_ptr<Renderer::Material> createMaterial(
        const std::string& materialName
    );

    static MaterialType detectMaterialType(const Properties& props);

    Attributes mAttributes;
    Properties mProperties;
};


#endif //NODEMESHLOADER_H
