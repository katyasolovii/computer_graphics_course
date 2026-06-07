//
// Created by Andrii Krenevych on 06.06.2025.
//

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ModelLoader.h"

#include "Node.h"
#include "NodeMesh.h"

#include "file_utils.h"
#include "Logger.h"
#include "math_utils.h"
#include "NodeMeshLoader.h"


ModelLoader::ModelLoader(std::string fileName)
    : mFileName(std::move(fileName)),
      mDirectory(getDirectory(mFileName)),
      mName(mFileName) {
}

std::shared_ptr<Renderer::Node> ModelLoader::load() {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        mFileName,
        aiProcess_Triangulate
        | aiProcess_ValidateDataStructure
        | aiProcess_SortByPType
        | aiProcess_GenNormals
        | aiProcess_FlipUVs
        | aiProcess_JoinIdenticalVertices
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Renderer::Logger::log().e("Assimp error: " + std::string(importer.GetErrorString()));
        return nullptr;
    }

    auto root = std::make_shared<Renderer::Node>(mName);
    const auto tree = processNode(scene->mRootNode, scene);
    root->addNode(tree);
    return root;
}

std::shared_ptr<Renderer::Node> ModelLoader::processNode(
    const aiNode* ai_node, const aiScene* scene) {
    auto nodeName = std::string(ai_node->mName.C_Str());
    std::shared_ptr<Renderer::Node> node;

    Renderer::Logger::log().d("Process node: " + nodeName);

    if (ai_node->mNumMeshes == 0 || ai_node->mNumMeshes > 1) {
        node = std::make_shared<Renderer::Node>(nodeName);

        for (unsigned int i = 0; i < ai_node->mNumMeshes; ++i) {
            const auto meshIndex = ai_node->mMeshes[i];
            const aiMesh* mesh = scene->mMeshes[meshIndex];
            node->addNode(NodeMeshLoader().create(scene, mesh, nodeName, mDirectory));
        }
    } else if (ai_node->mNumMeshes == 1) {
        const auto meshIndex = ai_node->mMeshes[0];
        const aiMesh* mesh = scene->mMeshes[meshIndex];
        node = NodeMeshLoader().create(scene, mesh, nodeName, mDirectory);
    }

    node->setTransformation(convert(ai_node->mTransformation));

    for (unsigned int i = 0; i < ai_node->mNumChildren; ++i) {
        std::shared_ptr<Renderer::Node> child = processNode(ai_node->mChildren[i], scene);
        node->addNode(child);
    }

    return node;
}
