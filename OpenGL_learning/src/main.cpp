#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_utils.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "OpenGL Rectangle Interpolation", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    std::string vertexShaderName = "res/shaders/triangle.vert";
    std::string fragmentShaderName = "res/shaders/triangle.frag";
    GLuint shaderProgram = createProgram(
        vertexShaderName,
        fragmentShaderName);

    // Координати вершин прямокутника (X, Y, Z)
    float vertices[] = {
        -0.5f,  0.5f, 0.0f,  // Вершина 1 (Верхній лівий кут)
         0.5f,  0.5f, 0.0f,  // Вершина 2 (Верхній правий кут)
        -0.5f, -0.5f, 0.0f,  // Вершина 3 (Нижній лівий кут)
         0.5f, -0.5f, 0.0f   // Вершина 4 (Нижній правий кут)
    };

    float colors[] = {
        1.0f, 0.0f, 0.0f,  // Червоний (Вершина 1)
        1.0f, 1.0f, 0.0f,  // Жовтий (Вершина 2)
        0.0f, 1.0f, 0.0f,  // Зелений (Вершина 3)
        0.0f, 0.0f, 1.0f   // Синій (Вершина 4)
    };

    GLuint VBO;
    GLuint VBO_color;
    GLuint VAO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_color);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // Буфер позицій
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint posAttribLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(
        posAttribLocation,        //  знайдена позиція aPos
        3,                        // 3 компоненти: x, y, z
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),        // крок (stride)
        (void*)0
    );
    glEnableVertexAttribArray(posAttribLocation);

    // Буфер кольорів
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint colorAttribLocation = glGetAttribLocation(shaderProgram, "aColor");
    glVertexAttribPointer(
        colorAttribLocation,      // знайдена позиція aColor
        3,                        // 3 компоненти: r, g, b
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),        // крок (stride)
        (void*)0
    );
    glEnableVertexAttribArray(colorAttribLocation);

    glBindVertexArray(0);

    /* Loop until the user closes the window */
    do
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO_color);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}