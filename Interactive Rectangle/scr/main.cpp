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

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(800, 800, "Interactive Rectangle", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSwapInterval(1);

    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

    std::string vertexShaderName = "res/shaders/rect.vert";
    std::string fragmentShaderName = "res/shaders/rect.frag";
    GLuint shaderProgram = createProgram(vertexShaderName, fragmentShaderName);

    GLint texture_loc = glGetUniformLocation(shaderProgram, "uTexture");
    GLint transform_loc = glGetUniformLocation(shaderProgram, "uTransformation");


    float vertices[] = {
        /* координати */ -0.6f, -0.3f,  /* текстурні координати */ 0.0f, 0.0f, // 0
        /* координати */  0.6f, -0.3f,  /* текстурні координати */ 1.0f, 0.0f, // 1
        /* координати */  0.6f,  0.3f,  /* текстурні координати */ 1.0f, 1.0f, // 2
        /* координати */ -0.6f,  0.3f,  /* текстурні координати */ 0.0f, 1.0f  // 3
    };

    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

    GLuint VBO, indexBuffer, VAO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &indexBuffer);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(glGetAttribLocation(shaderProgram, "aPos"), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram, "aPos"));

    glVertexAttribPointer(glGetAttribLocation(shaderProgram, "aUV"), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram, "aUV"));

    unsigned int texture = loadTexture("res/textures/texture_for_animation.jpg");

    float posX = 0.0f;
    float posY = 0.0f;
    float angle = 0.0f;
    float speed = 0.02f;

    do
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Реакція на дії користувача: КЛАВІАТУРА
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  posX -= speed;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) posX += speed;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    posY += speed;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  posY -= speed;

        // Реакція на дії користувача: МИША
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        float mouseX = (float)xpos / 400.0f - 1.0f;
        float mouseY = 1.0f - (float)ypos / 400.0f;

        if (mouseX >= posX - 0.6f && mouseX <= posX + 0.6f &&
            mouseY >= posY - 0.3f && mouseY <= posY + 0.3f)
        {
            angle += 3.0f;
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(posX, posY, 0.0f));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(texture_loc, 0);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
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