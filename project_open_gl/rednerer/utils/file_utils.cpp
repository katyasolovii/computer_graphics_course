//
// Created by Andrii Krenevych on 06.06.2025.
//

#include "file_utils.h"
#include <filesystem>
#include <string>
#include <algorithm>

namespace fs = std::filesystem;

std::string getDirectory(const std::string& filePath) {
    return fs::path(filePath).parent_path().string();
}

void normalizePath(std::string& path) {
#ifdef _WIN32
    std::replace(path.begin(), path.end(), '/', '\\');
#else
    std::replace(path.begin(), path.end(), '\\', '/');
#endif
}
