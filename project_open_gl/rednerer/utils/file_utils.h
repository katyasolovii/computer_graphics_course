//
// Created by Andrii Krenevych on 06.06.2025.
//

#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include <string>

std::string getDirectory(const std::string& filePath);

void normalizePath(std::string& texPath);

const std::string PATH_SEPARATOR =
#ifdef _WIN32
    "\\";
#else
        "/";
#endif

#endif //FILE_UTILS_H
