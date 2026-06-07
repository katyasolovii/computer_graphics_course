//
// Created by Andrii Krenevych on 16.06.2025.
//

#include "NodeMeshLoader.h"

#include "Constants.h"
#include "Logger.h"
#include "Material.h"
#include "MaterialType.h"
#include "Mesh.h"
#include "message.h"
#include "NodeMesh.h"
#include "texture_loader.h"
#include "assimp/scene.h"


NodeMeshLoader::NodeMeshLoader() = default;

std::shared_ptr<Renderer::NodeMesh> NodeMeshLoader::create(
    const aiScene* ai_scene,
    const aiMesh* ai_mesh,
    const std::string& name,
    const std::string& directory
) {
    auto nodeMesh = std::make_shared<Renderer::NodeMesh>(name);
    auto mesh = createMesh(ai_mesh);
    nodeMesh->setMesh(std::move(mesh));

    const auto materialIndex = ai_mesh->mMaterialIndex;
    const auto ai_material = ai_scene->mMaterials[materialIndex];
    const auto material = loadMaterial(ai_scene, ai_material, directory);

    nodeMesh->setMaterial(material);

    return nodeMesh;
}

std::unique_ptr<Renderer::Mesh> NodeMeshLoader::createMesh(const aiMesh* mesh) {
    Renderer::Logger::log().d("Process mesh " + std::string(mesh->mName.C_Str()));

    auto geometry = std::make_unique<Renderer::Mesh>();

    std::vector<float> vertexData;
    std::vector<unsigned int> indices;

    mAttributes = Attributes();
    auto& attributes = mAttributes.attributes;

    attributes.push_back({ATTRIB_POSITION, 3});

    // Нормалі
    if (mesh->HasNormals()) {
        attributes.push_back({ATTRIB_NORMAL, 3});
        Renderer::Logger::log().d("Found attribute " + std::string(ATTRIB_NORMAL));
    }
    if (mesh->HasTextureCoords(0)) {
        attributes.push_back({ATTRIB_TEXTURE_UV, 2});
        Renderer::Logger::log().d("Found attribute " + std::string(ATTRIB_TEXTURE_UV));
    }
    if (mesh->HasTangentsAndBitangents()) {
        attributes.push_back({ATTRIB_TANGENT, 3});
        attributes.push_back({ATTRIB_BITANGENT, 3});

        Renderer::Logger::log().d("Found attribute " + std::string(ATTRIB_TANGENT));
        Renderer::Logger::log().d("Found attribute " + std::string(ATTRIB_BITANGENT));
    }
    if (mesh->HasVertexColors(0)) {
        attributes.push_back({ATTRIB_COLOR, 4});
        Renderer::Logger::log().d("Found attribute " + std::string(ATTRIB_COLOR));
    }

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        // Позиція
        vertexData.push_back(mesh->mVertices[i].x);
        vertexData.push_back(mesh->mVertices[i].y);
        vertexData.push_back(mesh->mVertices[i].z);

        if (mesh->HasNormals()) {
            vertexData.push_back(mesh->mNormals[i].x);
            vertexData.push_back(mesh->mNormals[i].y);
            vertexData.push_back(mesh->mNormals[i].z);
        }

        if (mesh->HasTextureCoords(0)) {
            vertexData.push_back(mesh->mTextureCoords[0][i].x);
            vertexData.push_back(mesh->mTextureCoords[0][i].y);
        }

        // Тангенти
        if (mesh->HasTangentsAndBitangents()) {
            vertexData.push_back(mesh->mTangents[i].x);
            vertexData.push_back(mesh->mTangents[i].y);
            vertexData.push_back(mesh->mTangents[i].z);

            vertexData.push_back(mesh->mBitangents[i].x);
            vertexData.push_back(mesh->mBitangents[i].y);
            vertexData.push_back(mesh->mBitangents[i].z);
        }

        if (mesh->HasVertexColors(0)) {
            vertexData.push_back(mesh->mColors[0][i].r);
            vertexData.push_back(mesh->mColors[0][i].g);
            vertexData.push_back(mesh->mColors[0][i].b);
            vertexData.push_back(mesh->mColors[0][i].a);
        }
    }

    // Індекси
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    // Передати у Geometry
    geometry->setVertices(std::move(vertexData));
    if (!indices.empty()) {
        geometry->setIndices(std::move(indices));
    }

    // Опис атрибутів
    for (const auto& attr: attributes) {
        geometry->addAttribute(attr.name, attr.size);
    }

    return geometry;
}

std::vector<std::string> NodeMeshLoader::generatePropertyDefines() {
    std::vector<std::string> defines;

    // --- Текстури ---
    if (mProperties.hasTexture(TEXTURE_DIFFUSE)) defines.emplace_back("HAS_DIFFUSE_MAP");
    if (mProperties.hasTexture(TEXTURE_SPECULAR)) defines.emplace_back("HAS_SPECULAR_MAP");
    if (mProperties.hasTexture(TEXTURE_AMBIENT)) defines.emplace_back("HAS_AMBIENT_MAP");
    if (mProperties.hasTexture(TEXTURE_EMISSIVE)) defines.emplace_back("HAS_EMISSIVE_MAP");
    if (mProperties.hasTexture(TEXTURE_HEIGHT)) defines.emplace_back("HAS_HEIGHT_MAP");
    if (mProperties.hasTexture(TEXTURE_NORMAL)) defines.emplace_back("HAS_NORMAL_MAP");
    if (mProperties.hasTexture(TEXTURE_SHININESS)) defines.emplace_back("HAS_SHININESS_MAP");
    if (mProperties.hasTexture(TEXTURE_OPACITY)) defines.emplace_back("HAS_OPACITY_MAP");
    if (mProperties.hasTexture(TEXTURE_DISPLACEMENT)) defines.emplace_back("HAS_DISPLACEMENT_MAP");
    if (mProperties.hasTexture(TEXTURE_LIGHTMAP)) defines.emplace_back("HAS_LIGHTMAP_MAP");
    if (mProperties.hasTexture(TEXTURE_REFLECTION)) defines.emplace_back("HAS_REFLECTION_MAP");
    if (mProperties.hasTexture(TEXTURE_BASE_COLOR)) defines.emplace_back("HAS_BASECOLOR_MAP");
    if (mProperties.hasTexture(TEXTURE_METALNESS)) defines.emplace_back("HAS_METALNESS_MAP");
    if (mProperties.hasTexture(TEXTURE_ROUGHNESS)) defines.emplace_back("HAS_ROUGHNESS_MAP");
    if (mProperties.hasTexture(TEXTURE_AO)) defines.emplace_back("HAS_AO_MAP");

    // --- Матеріальні властивості ---
    if (mProperties.hasProperty(MATERIAL_COLOR_DIFFUSE)) defines.emplace_back("HAS_COLOR_DIFFUSE");
    if (mProperties.hasProperty(MATERIAL_COLOR_SPECULAR)) defines.emplace_back("HAS_COLOR_SPECULAR");
    if (mProperties.hasProperty(MATERIAL_COLOR_AMBIENT)) defines.emplace_back("HAS_COLOR_AMBIENT");
    if (mProperties.hasProperty(MATERIAL_COLOR_EMISSIVE)) defines.emplace_back("HAS_COLOR_EMISSIVE");
    if (mProperties.hasProperty(MATERIAL_COLOR_TRANSPARENT)) defines.emplace_back("HAS_COLOR_TRANSPARENT");
    if (mProperties.hasProperty(MATERIAL_COLOR_REFLECTIVE)) defines.emplace_back("HAS_COLOR_REFLECTIVE");

    if (mProperties.hasProperty(MATERIAL_OPACITY)) defines.emplace_back("HAS_OPACITY_FACTOR");
    if (mProperties.hasProperty(MATERIAL_SHININESS)) defines.emplace_back("HAS_SHININESS_FACTOR");
    if (mProperties.hasProperty(MATERIAL_SHININESS_STRENGTH))defines.emplace_back("HAS_SHININESS_STRENGTH");
    if (mProperties.hasProperty(MATERIAL_REFRACTI)) defines.emplace_back("HAS_REFRACTION_INDEX");
    if (mProperties.hasProperty(MATERIAL_TWO_SIDED)) defines.emplace_back("HAS_TWO_SIDED");

    if (mProperties.hasProperty(MATERIAL_METALLIC_FACTOR)) defines.emplace_back("HAS_METALLIC_FACTOR");
    if (mProperties.hasProperty(MATERIAL_ROUGHNESS_FACTOR)) defines.emplace_back("HAS_ROUGHNESS_FACTOR");
    if (mProperties.hasProperty(MATERIAL_AO_FACTOR)) defines.emplace_back("HAS_AO_FACTOR");

    return defines;
}

std::vector<std::string> NodeMeshLoader::generateAttributeDefines() {
    std::vector<std::string> defines;

    if (mAttributes.hasAttribute(ATTRIB_POSITION)) defines.emplace_back("HAS_ATTRIB_POSITION");
    if (mAttributes.hasAttribute(ATTRIB_NORMAL)) defines.emplace_back("HAS_ATTRIB_NORMAL");
    if (mAttributes.hasAttribute(ATTRIB_TEXTURE_UV)) defines.emplace_back("HAS_ATTRIB_UV");
    if (mAttributes.hasAttribute(ATTRIB_TANGENT)) defines.emplace_back("HAS_ATTRIB_TANGENT");
    if (mAttributes.hasAttribute(ATTRIB_BITANGENT)) defines.emplace_back("HAS_ATTRIB_BITANGENT");
    if (mAttributes.hasAttribute(ATTRIB_COLOR)) defines.emplace_back("HAS_ATTRIB_COLOR");

    return defines;
}

///////////// Material

std::vector<std::pair<aiTextureType, std::string> > textureMap = {
    {aiTextureType_DIFFUSE, TEXTURE_DIFFUSE},
    {aiTextureType_NORMALS, TEXTURE_NORMAL},
    {aiTextureType_SPECULAR, TEXTURE_SPECULAR},
    {aiTextureType_EMISSIVE, TEXTURE_EMISSIVE},
    {aiTextureType_HEIGHT, TEXTURE_HEIGHT},
    {aiTextureType_SHININESS, TEXTURE_SHININESS},
    {aiTextureType_OPACITY, TEXTURE_OPACITY},
    {aiTextureType_DISPLACEMENT, TEXTURE_DISPLACEMENT},
    {aiTextureType_BASE_COLOR, TEXTURE_BASE_COLOR},
    {aiTextureType_METALNESS, TEXTURE_METALNESS},
    {aiTextureType_DIFFUSE_ROUGHNESS, TEXTURE_ROUGHNESS},
    {aiTextureType_AMBIENT_OCCLUSION, TEXTURE_AO},
};

std::shared_ptr<Renderer::Material> NodeMeshLoader::loadMaterial(
    const aiScene* scene,
    const aiMaterial* ai_material,
    const std::string& modelDirectory
) {
    parseMaterialProperties(scene, ai_material, modelDirectory);

    return createMaterial(ai_material->GetName().C_Str());
}

void NodeMeshLoader::parseMaterialProperties(
    const aiScene* scene,
    const aiMaterial* aiMat,
    const std::string& modelDirectory
) {
    mProperties = Properties();
    auto& [textures, props4f, props1f, props1i] = mProperties;

    for (auto& [assimpType, engineType]: textureMap) {
        if (const auto tex = loadTextureFromAssimp(scene, aiMat, assimpType, modelDirectory)) {
            textures[engineType] = tex;
            Renderer::Logger::log().d("Texture type: " + engineType);
        }
    }

    //////////// Parsing properties /////////////
    aiColor4D color;

    if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
        props4f[MATERIAL_COLOR_DIFFUSE] = {color.r, color.g, color.b, color.a};
        Renderer::Logger::log().d(messageProperty(MATERIAL_COLOR_DIFFUSE, props4f[MATERIAL_COLOR_DIFFUSE]));
    }

    if (aiMat->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
        props4f[MATERIAL_COLOR_SPECULAR] = {color.r, color.g, color.b, color.a};
        Renderer::Logger::log().d(messageProperty(MATERIAL_COLOR_SPECULAR, props4f[MATERIAL_COLOR_SPECULAR]));
    }

    if (aiMat->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
        props4f[MATERIAL_COLOR_AMBIENT] = {color.r, color.g, color.b, color.a};
        Renderer::Logger::log().d(messageProperty(MATERIAL_COLOR_AMBIENT, props4f[MATERIAL_COLOR_AMBIENT]));
    }

    if (aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS) {
        props4f[MATERIAL_COLOR_EMISSIVE] = {color.r, color.g, color.b, color.a};
        Renderer::Logger::log().d(messageProperty(MATERIAL_COLOR_EMISSIVE, props4f[MATERIAL_COLOR_EMISSIVE]));
    }

    if (aiMat->Get(AI_MATKEY_COLOR_TRANSPARENT, color) == AI_SUCCESS) {
        props4f[MATERIAL_COLOR_TRANSPARENT] = {color.r, color.g, color.b, color.a};
        Renderer::Logger::log().d(messageProperty(MATERIAL_COLOR_TRANSPARENT, props4f[MATERIAL_COLOR_TRANSPARENT]));
    }

    if (aiMat->Get(AI_MATKEY_COLOR_REFLECTIVE, color) == AI_SUCCESS) {
        props4f[MATERIAL_COLOR_REFLECTIVE] = {color.r, color.g, color.b, color.a};
        Renderer::Logger::log().d(messageProperty(MATERIAL_COLOR_REFLECTIVE, props4f[MATERIAL_COLOR_REFLECTIVE]));
    }

    float value;
    if (aiMat->Get(AI_MATKEY_OPACITY, value) == AI_SUCCESS) {
        props1f[MATERIAL_OPACITY] = value;
        Renderer::Logger::log().d(messageProperty(MATERIAL_OPACITY, value));
    }

    if (aiMat->Get(AI_MATKEY_SHININESS, value) == AI_SUCCESS) {
        props1f[MATERIAL_SHININESS] = value;
        Renderer::Logger::log().d(messageProperty(MATERIAL_SHININESS, value));
    }

    if (aiMat->Get(AI_MATKEY_SHININESS_STRENGTH, value) == AI_SUCCESS) {
        props1f[MATERIAL_SHININESS_STRENGTH] = value;
        Renderer::Logger::log().d(messageProperty(MATERIAL_SHININESS_STRENGTH, value));
    }

    if (aiMat->Get(AI_MATKEY_REFRACTI, value) == AI_SUCCESS) {
        props1f[MATERIAL_REFRACTI] = value;
        Renderer::Logger::log().d(messageProperty(MATERIAL_REFRACTI, value));
    }

    int intVal;
    if (aiMat->Get(AI_MATKEY_TWOSIDED, intVal) == AI_SUCCESS) {
        props1i[MATERIAL_TWO_SIDED] = intVal;
        Renderer::Logger::log().d(messageProperty(MATERIAL_TWO_SIDED, intVal));
    }
}

std::shared_ptr<Renderer::Material> NodeMeshLoader::createMaterial(
    const std::string& materialName
) {
    auto& [textures, props4f, props1f, props1i] = mProperties;

    auto propDefines = generatePropertyDefines();
    auto attrDefines = generateAttributeDefines();

    auto materialType = detectMaterialType(mProperties);
    Renderer::Logger::log().d("Material type: " + toString(materialType));
    std::cout << materialType << std::endl;

    auto material = std::make_shared<Renderer::Material>(materialName);

    std::shared_ptr<Renderer::Program> shaderProgram;
    if (materialType == MaterialType::Phong) {
        shaderProgram = std::make_shared<Renderer::Program>(
            "res/shaders/MateriaPhong.vert",
            "res/shaders/MateriaPhong.frag",
            attrDefines,
            propDefines
        );
    } else {
        shaderProgram = std::make_shared<Renderer::Program>(
            "res/shaders/colored_only.vert",
            "res/shaders/colored_only.frag",
            attrDefines,
            propDefines
        );
    }

    material->setProgram(shaderProgram);

    for (const auto& [texName, texture]: textures) {
        material->setTexture(texName, texture);
    }

    for (const auto& [propName, prop]: props4f) {
        material->setProperty(propName, prop);
    }

    for (const auto& [propName, prop]: props1f) {
        material->setProperty(propName, prop);
    }

    for (const auto& [propName, prop]: props1f) {
        material->setProperty(propName, prop);
    }

    return material;
}


MaterialType NodeMeshLoader::detectMaterialType(const Properties& props) {
    auto get1f = [&](const std::string& name, float def = 0.0f) -> float {
        const auto it = props.props1f.find(name);
        return (it != props.props1f.end()) ? it->second : def;
    };

    auto get1i = [&](const std::string& name, int def = 0) -> int {
        const auto it = props.props1i.find(name);
        return (it != props.props1i.end()) ? it->second : def;
    };

    // === Unlit ===
    if (props.hasTexture(TEXTURE_EMISSIVE) &&
        !props.hasTexture(TEXTURE_DIFFUSE) && !props.hasProperty(MATERIAL_COLOR_DIFFUSE))
        return MaterialType::Unlit;

    // === Glass ===
    if (get1f(MATERIAL_OPACITY, 1.0f) < 1.0f &&
        get1f(MATERIAL_REFRACTI, 1.0f) > 1.0f)
        return MaterialType::Glass;

    // === Translucent ===
    if (get1f(MATERIAL_OPACITY, 1.0f) < 1.0f)
        return MaterialType::Translucent;

    // === Toon === (кастомний прапор)
    if (get1i("uToon", 0) == 1)
        return MaterialType::Toon;

    // === PBR Metal/Rough ===
    if (props.hasTexture(TEXTURE_METALNESS) || props.hasTexture(TEXTURE_ROUGHNESS) ||
        get1f(MATERIAL_METALLIC_FACTOR) > 0.0f || get1f(MATERIAL_ROUGHNESS_FACTOR) > 0.0f)
        return MaterialType::PBR_MetalRoughness;

    // === PBR Specular/Gloss ===
    if (props.hasTexture(TEXTURE_SPECULAR) &&
        (props.hasTexture(TEXTURE_HEIGHT) || get1f(MATERIAL_SHININESS) > 0.0f))
        return MaterialType::PBR_SpecGloss;

    // === Phong ===
    if (props.hasProperty(MATERIAL_COLOR_SPECULAR) || get1f(MATERIAL_SHININESS) > 0.0f)
        return MaterialType::Phong;

    // === Lambert ===
    if (!props.hasTexture(TEXTURE_DIFFUSE) &&
        !props.hasTexture(TEXTURE_SPECULAR) &&
        props.hasProperty(MATERIAL_COLOR_DIFFUSE)
    )
        return MaterialType::Lambert;

    // === Unknown ===
    return MaterialType::Unknown;
}
