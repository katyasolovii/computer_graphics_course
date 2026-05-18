#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_utils.h"
#include "texture.h"

#include <../glm/glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>
#include <../glm/gtc/type_ptr.hpp>


glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 6.0f); // Позиція спостереження
float yaw   = -90.0f; // Кут повороту (вліво-вправо)
float pitch =  0.0f;  // Кут нахилу (вгору-вниз)

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Буфер трафарет (необхідно для малювання рамки)
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    auto width = 1280;
    auto height = 780;
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(width, height, "Working with 3D. Depth and stencil buffers", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        return -1;
    }

    glfwSwapInterval(1); // синхронізує рендер-цикл з частотою екрану

    // Налаштування тестів та кольору фону
    glEnable(GL_DEPTH_TEST);   // Тест глибини для коректного 3D
    glEnable(GL_STENCIL_TEST); // Тест трафарету для малювання рамки
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Налаштування операцій трафарету
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    std::string vShader = "res/shaders/rect.vert";
    std::string fShader = "res/shaders/rect.frag";
    GLuint shaderProgram = createProgram(vShader, fShader);

    // Формат на вершину: Position(3) + UV(2) = 5 floats
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 0, 4, 7, 7, 3, 0,
        1, 5, 6, 6, 2, 1, 3, 2, 6, 6, 7, 3, 0, 1, 5, 5, 4, 0
    };


    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // stride: 5 float-ів на вершину
    const int STRIDE = 5 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, STRIDE, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texs[] = {
        loadTexture("../res/textures/t1.jpg"),
        loadTexture("../res/textures/t2.jpg"),
        loadTexture("../res/textures/t3.jpg")
    };

    glm::vec3 positions[] = {
        glm::vec3(0.0f, 1.0f, -2.0f),
        glm::vec3(-1.8f, -1.0f, -2.0f),
        glm::vec3(1.8f, -1.0f, -2.0f)
    };
    float scales[] = { 1.2f, 0.8f, 1.0f };
    float cubeAngles[] = { 0.0f, 0.0f, 0.0f };

    GLint trans_loc = glGetUniformLocation(shaderProgram, "uTransformation");
    GLint tex_loc = glGetUniformLocation(shaderProgram, "uTexture0");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Керування орієнтацією камери (W, S, A, D)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pitch += 1.5f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pitch -= 1.5f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) yaw -= 1.5f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) yaw += 1.5f;

        if (pitch > 89.0f) pitch = 89.0f; if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        glm::vec3 cameraFront = glm::normalize(front);

        // Керування позицією камери (Стрілочки)
        float speed = 0.05f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    cameraPos += speed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  cameraPos -= speed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, glm::vec3(0,1,0))) * speed;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, glm::vec3(0,1,0))) * speed;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glUseProgram(shaderProgram);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0, 1, 0));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/height, 0.1f, 100.0f);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        for (int i = 0; i < 3; i++) {
            bool isActive = false;

            // Активація клавіатурою
            if ((i == 0 && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) ||
                (i == 1 && glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) ||
                (i == 2 && glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)) isActive = true;

            // Активація мишкою: тільки якщо курсор безпосередньо в зоні куба
            if (i == 0 && (xpos > 500 && xpos < 780 && ypos > 50 && ypos < 300)) isActive = true;
            if (i == 1 && (xpos > 150 && xpos < 450 && ypos > 450 && ypos < 700)) isActive = true;
            if (i == 2 && (xpos > 850 && xpos < 1150 && ypos > 450 && ypos < 700)) isActive = true;

            // Якщо мишка НЕ на кубі (і не натиснута клавіша), isActive буде false, куб зупиниться
            if (isActive) cubeAngles[i] += 0.05f;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            model = glm::scale(model, glm::vec3(scales[i]));
            model = glm::rotate(model, cubeAngles[i], glm::vec3(0.5f, 1.0f, 0.0f));

            // Рендеринг з використанням трафарету
            if (isActive) {
                glStencilFunc(GL_ALWAYS, 1, 0xFF); // Завжди проходити тест, писати 1
                glStencilMask(0xFF); // Дозволити запис
            } else {
                glStencilMask(0x00); // Заборонити запис для неактивних
            }

            glUniformMatrix4fv(trans_loc, 1, GL_FALSE, glm::value_ptr(proj * view * model));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texs[i]);
            glUniform1i(tex_loc, 0);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

            // Малювання рамки навколо активного куба
            if (isActive) {
                glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Малювати ТІЛЬКИ там, де немає куба
                glStencilMask(0x00); // Заборонити зміну трафарету
                glDisable(GL_DEPTH_TEST); // Малювати рамку поверх

                glm::mat4 outline = glm::scale(model, glm::vec3(1.05f));
                glUniformMatrix4fv(trans_loc, 1, GL_FALSE, glm::value_ptr(proj * view * outline));

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                glEnable(GL_DEPTH_TEST);
                glStencilMask(0xFF);
                glStencilFunc(GL_ALWAYS, 0, 0xFF);
            }
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}