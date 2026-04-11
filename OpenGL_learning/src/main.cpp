#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader_utils.h"

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Rectangle Interpolation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    // Завантаження шейдерів
    std::string vShader = "res/shaders/triangle.vert";
    std::string fShader = "res/shaders/triangle.frag";
    GLuint shaderProgram = createProgram(vShader, fShader);

    // Координати вершин прямокутника (X, Y, Z)
    float vertices[] = {
        -0.5f,  0.5f, 0.0f,  // Вершина 1 (Верхній лівий кут)
         0.5f,  0.5f, 0.0f,  // Вершина 2 (Верхній правий кут)
        -0.5f, -0.5f, 0.0f,  // Вершина 3 (Нижній лівий кут)
         0.5f, -0.5f, 0.0f   // Вершина 4 (Нижній правий кут)
    };

    // Кольори для кожної вершини (R, G, B)
    float colors[] = {
        1.0f, 0.0f, 0.0f,  // Червоний (Вершина 1)
        1.0f, 1.0f, 0.0f,  // Жовтий (Вершина 2)
        0.0f, 1.0f, 0.0f,  // Зелений (Вершина 3)
        0.0f, 0.0f, 1.0f   // Синій (Вершина 4)
    };

    GLuint VAO, VBO_pos, VBO_color;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_pos);
    glGenBuffers(1, &VBO_color);

    glBindVertexArray(VAO);

    // Буфер позицій
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Буфер кольорів
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_pos);
    glDeleteBuffers(1, &VBO_color);
    glfwTerminate();
    return 0;
}