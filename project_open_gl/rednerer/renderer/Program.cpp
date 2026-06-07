//
// Created by Andrii Krenevych on 21.04.2025.
//

#include "Program.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "Constants.h"
#include "Logger.h"
#include "glad/glad.h"

namespace Renderer {
    Program::Program(
        const std::string& vertPath,
        const std::string& fragPath)
        : mShaderProgram(0) {
        mShaderProgram = createProgram(vertPath, fragPath);
    }

    Program::Program(
        const std::string& vertPath,
        const std::string& fragPath,
        const std::vector<std::string>& attrDefines,
        const std::vector<std::string>& propDefines)
        : mShaderProgram(0) {
        auto materialDefines = std::vector<std::string>();
        materialDefines.insert(materialDefines.end(), attrDefines.begin(), attrDefines.end());
        materialDefines.insert(materialDefines.end(), propDefines.begin(), propDefines.end());

        mShaderProgram = createProgram(vertPath, fragPath, materialDefines);
    }

    Program::~Program() {
        if (mShaderProgram) {
            glDeleteProgram(mShaderProgram);
        }
    }

    unsigned int Program::getShaderProgram() const {
        return mShaderProgram;
    }

    void Program::activate() const {
        Logger::log().d("Program::activate: id = " + std::to_string(mShaderProgram));

        glUseProgram(mShaderProgram);
    }

    void Program::deactivate() const {
        Logger::log().d("Program::deactivate: id = " + std::to_string(mShaderProgram));

        glUseProgram(0);
    }

    GLuint Program::shader(const std::string& filePath, const GLuint shaderType) {
        std::string shaderCodeStr = LoadShaderFromFile(filePath);
        auto shaderCode = shaderCodeStr.c_str();

        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderCode, nullptr);
        glCompileShader(shader);

        // Перевірка компіляції шейдера
        GLint success;

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            Logger::log().e("Помилка компіляції шейдера:\n" + filePath + "\n" + infoLog);
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    GLuint Program::createProgram(
        const std::string& vertPath,
        const std::string& fragPath
    ) {
        auto vertexShader = createShader(
            vertPath,
            GL_VERTEX_SHADER
        );

        if (vertexShader == 0) {
            return 0;
        }

        auto fragmentShader = createShader(
            fragPath,
            GL_FRAGMENT_SHADER
        );

        if (fragmentShader == 0) {
            return 0;
        }

        const GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Перевірка лінкування шейдерної програми
        GLint success;

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            Logger::log().e("Помилка лінкування shader program:\n" + std::string(infoLog));
            glDeleteProgram(shaderProgram);
            return 0;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }

    GLuint Program::createProgram(
        const std::string& vertPath,
        const std::string& fragPath,
        const std::vector<std::string>& defines
    ) {
        std::string vertexShaderCodeStr = LoadShaderFromFile(vertPath);
        std::string fragmentShaderCodeStr = LoadShaderFromFile(fragPath);

        const auto vertexShader = compileShader(vertexShaderCodeStr, GL_VERTEX_SHADER, defines);
        if (vertexShader == 0)
            return 0;

        const auto fragmentShader = compileShader(fragmentShaderCodeStr, GL_FRAGMENT_SHADER, defines);
        if (fragmentShader == 0)
            return 0;

        const GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Перевірка лінкування шейдерної програми
        GLint success;

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            Logger::log().e("Помилка лінкування shader program:\n" + std::string(infoLog));
            glDeleteProgram(shaderProgram);
            return 0;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }


    GLuint Program::compileShader(
        const std::string& shaderCode,
        GLenum shaderType,
        const std::vector<std::string>& defines
    ) {
        std::string defineBlock = generateShaderDefines(defines);

        const char* sources[] = {
            GLSL_VERSION,
            defineBlock.c_str(),
            shaderCode.c_str()
        };

        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 3, sources, nullptr);
        glCompileShader(shader);

        // Перевірка компіляції
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[1024];
            glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            Logger::log().e("Помилка компіляції шейдера:\n" + std::string(infoLog));
            glDeleteShader(shader);
        }

        return shader;
    }

    GLuint Program::createShader(const std::string& filePath, const GLuint shaderType) {
        const std::string shaderCodeStr = LoadShaderFromFile(filePath);
        if (shaderCodeStr.empty()) {
            return 0;
        }

        const auto shaderCode = shaderCodeStr.c_str();

        const GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderCode, nullptr);
        glCompileShader(shader);

        // Перевірка компіляції шейдера
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            Logger::log().e("Помилка компіляції шейдера:\n" + filePath + "\n" + std::string(infoLog));
            glDeleteShader(shader);
        }

        return shader;
    }

    std::string Program::LoadShaderFromFile(const std::string& filePath) {
        std::ifstream shaderFile(filePath);
        if (!shaderFile.is_open()) {
            Logger::log().e("Не вдалося відкрити файл: " + filePath);
            return "";
        }

        std::stringstream buffer;
        buffer << shaderFile.rdbuf(); // зчитує весь вміст файлу в потік

        return buffer.str(); // повертає як std::string
    }

    std::string Program::generateShaderDefines(const std::vector<std::string>& defines) {
        std::string prefix;
        for (const std::string& def: defines) {
            prefix += "#define " + def + "\n";
        }
        return prefix;
    }
} // Renderer
