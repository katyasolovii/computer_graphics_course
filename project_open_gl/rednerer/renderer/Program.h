//
// Created by Andrii Krenevych on 21.04.2025.
//

#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <iostream>
#include <vector>

#include "glad/glad.h"


namespace Renderer {
    class Program {
    public:
        Program(
            const std::string& vertPath,
            const std::string& fragPath
        );

        Program(
            const std::string& vertPath,
            const std::string& fragPath,
            const std::vector<std::string>& attrDefines,
            const std::vector<std::string>& propDefines
        );

        virtual ~Program();

        [[nodiscard]] unsigned int getShaderProgram() const;

        void activate() const;

        void deactivate() const;

    private:
        static GLuint shader(const std::string& filePath, GLuint shaderType);

        static GLuint createProgram(
            const std::string& vertPath,
            const std::string& fragPath
        );

        static GLuint createProgram(
            const std::string& vertPath,
            const std::string& fragPath,
            const std::vector<std::string>& defines
        );

        static GLuint compileShader(
            const std::string& shaderCode,
            GLenum shaderType,
            const std::vector<std::string>& defines
        );

        static GLuint createShader(
            const std::string& filePath,
            GLuint shaderType);


        static std::string LoadShaderFromFile(const std::string& filePath);

        static std::string generateShaderDefines(const std::vector<std::string>& defines);

        GLuint mShaderProgram;
    };
} // Renderer

#endif //PROGRAM_H
