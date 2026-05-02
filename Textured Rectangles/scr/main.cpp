//
// Created by Katya Solovii on 02.05.2026.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../utils/shader_utils.h"
#include "../utils/texture.h"

#include <../glm/glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>
#include <../glm/gtc/type_ptr.hpp>

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
    window = glfwCreateWindow(800, 600, "Three Textured Rectangles", NULL, NULL);
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

    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

    std::string vertexShaderName = "res/shaders/rect.vert";
    std::string fragmentShaderName = "res/shaders/rect.frag";
    GLuint shaderProgram = createProgram(vertexShaderName, fragmentShaderName);

    GLint texture_loc = glGetUniformLocation(shaderProgram, "uTexture");
    GLint transform_loc = glGetUniformLocation(shaderProgram, "uTransformation");

    float vertices[] = {
        /* координати */ -0.2f, -0.4f,  /* текстурні координати */ 0.0f, 0.0f, // 0
        /* координати */  0.2f, -0.4f,  /* текстурні координати */ 1.0f, 0.0f, // 1
        /* координати */  0.2f,  0.4f,  /* текстурні координати */ 1.0f, 1.0f, // 2
        /* координати */ -0.2f,  0.4f,  /* текстурні координати */ 0.0f, 1.0f  // 3
    };

    unsigned int indices[] = {
        0, 1, 2, // перший трикутник
        0, 2, 3  // другий трикутник
    };

    GLuint VBO, indexBuffer;
    GLuint VAO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &indexBuffer);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint posAttribLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(
        posAttribLocation,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(posAttribLocation);

    GLuint textureCoordsAttribLocation = glGetAttribLocation(shaderProgram, "aUV");
    glVertexAttribPointer(
        textureCoordsAttribLocation,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)(2 * sizeof(float))
    );
    glEnableVertexAttribArray(textureCoordsAttribLocation);

    glBindVertexArray(0);

    unsigned int texture0 = loadTexture("res/textures/texture_1.jpeg");
    unsigned int texture1 = loadTexture("res/textures/texture_2.jpg");
    unsigned int texture2 = loadTexture("res/textures/texture_3.jpg");

    do
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.6f, 0.0f, 0.0f));
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(model1));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glUniform1i(texture_loc, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glm::mat4 model2 = glm::mat4(1.0f);
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(model2));
        glBindTexture(GL_TEXTURE_2D, texture1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glm::mat4 model3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.6f, 0.0f, 0.0f));
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(model3));
        glBindTexture(GL_TEXTURE_2D, texture2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &indexBuffer);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture0);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);

    glfwTerminate();
    return 0;
}