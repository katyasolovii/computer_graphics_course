//
// Created by Andrii Krenevych on 06.06.2025.
//

#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <string>
#include <memory>

struct aiScene;
struct aiNode;

namespace Renderer {
    class Node;
}

class ModelLoader {
public:
    explicit ModelLoader(std::string fileName);

    ~ModelLoader() = default;

    std::shared_ptr<Renderer::Node> load();

private:
    std::string mFileName;
    std::string mDirectory;
    std::string mName;

    std::shared_ptr<Renderer::Node> processNode(
        const aiNode* ai_node,
        const aiScene* scene);
};


#endif //MODEL_LOADER_H
