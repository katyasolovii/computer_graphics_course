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
    window = glfwCreateWindow(600, 600, "Rotating Textured Square", NULL, NULL);
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

    /* Обмеження кількості кадрів анімації (FPS) шістдесятма кадрами на секунду */
    glfwSwapInterval(1);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    std::string vertexShaderName = "res/shaders/rect.vert";
    std::string fragmentShaderName = "res/shaders/rect.frag";
    GLuint shaderProgram = createProgram(vertexShaderName, fragmentShaderName);

    GLint texture_loc = glGetUniformLocation(shaderProgram, "uTexture");
    GLint transform_loc = glGetUniformLocation(shaderProgram, "uTransformation");

    float vertices[] = {
        /* координати */  -0.5f, -0.5f,  /* текстурні координати */  0.0f, 0.0f, // 0
        /* координати */   0.5f, -0.5f,  /* текстурні координати */  1.0f, 0.0f, // 1
        /* координати */   0.5f,  0.5f,  /* текстурні координати */  1.0f, 1.0f, // 2
        /* координати */  -0.5f,  0.5f,  /* текстурні координати */  0.0f, 1.0f, // 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,
    };

    GLuint VBO, indexBuffer; // data - ідентифікатор для даних - місток CPU та GPU
    GLuint VAO; // vertex array object

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &indexBuffer);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind = activate
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint posAttribLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(
        posAttribLocation,   // позиція атрибуту у шейдері
        2,                   // 2 компоненти: x, y
        GL_FLOAT,            // тип даних
        GL_FALSE,            // не нормалізувати
        4 * sizeof(float),   // stride: 4 float-а на вершину (x, y, u, v)
        (void*)0             // offset: починаємо з 0
    );
    glEnableVertexAttribArray(posAttribLocation);

    GLuint textureCoordsAttribLocation = glGetAttribLocation(shaderProgram, "aUV");
    glVertexAttribPointer(
        textureCoordsAttribLocation, // позиція атрибуту у шейдері
        2,                           // 2 компоненти: u, v
        GL_FLOAT,                    // тип даних
        GL_FALSE,                    // не нормалізувати
        4 * sizeof(float),           // stride: 4 float-а на вершину
        (void*)(2 * sizeof(float))   // offset: починаємо з 2 (після x, y)
    );
    glEnableVertexAttribArray(textureCoordsAttribLocation);

    glBindVertexArray(0); // деактивувати VAO

    unsigned int texture = loadTexture("res/textures/texture_for_animation.jpg");

    // Змінні для анімації та контролю паузи
    float angle = 0.0f;
    bool isPaused = false;
    bool spacePressedLastFrame = false;

    /* Loop until the user closes the window */
    do
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        /* Можливість зупинки та продовження анімації натиском SPACE */
        bool spacePressed = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
        if (spacePressed && !spacePressedLastFrame) {
            isPaused = !isPaused; // Перемикаємо стан
        }
        spacePressedLastFrame = spacePressed;

        if (!isPaused) {
            angle += 1.0f;
        }

        glm::mat4 model = glm::mat4(1.0f);
        // Обертання навколо центру (0,0,1)
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(texture_loc, 0);

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &indexBuffer);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture);

    glfwTerminate();
    return 0;
}