#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "InputDispatcher.h"
#include "Logger.h"
#include "SceneManager.h"

std::unique_ptr<Renderer::SceneManager> createSceneManager();
std::unique_ptr<Renderer::SceneManager> sceneManager;

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
    glViewport(0, 0, width, height); // оновлення області рендеру

    if (sceneManager != nullptr) {
        sceneManager->onViewportChange(width, height);
    }
}

int main(void) {
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // для macOS


    auto width = 1280;
    auto height = 720;
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Sphere", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        Renderer::Logger::log().e("Failed to initialize GLAD");
        return -1;
    }
    glfwSwapInterval(1);

    sceneManager = createSceneManager();
    sceneManager->onCreate();
    sceneManager->onViewportChange(width, height);

    // Встановлюємо callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, [](GLFWwindow*, const int key, const int scancode, const int action, int mods) {
        Renderer::InputDispatcher::getInstance().dispatchKey(key, scancode, action, mods);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow*, const int button, const int action, int mods) {
        double xPos, yPos;
        glfwGetCursorPos(glfwGetCurrentContext(), &xPos, &yPos);

        Renderer::InputDispatcher::getInstance().dispatchMouseButton(button, xPos, yPos ,action, mods);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow*, const double xPos, const double yPos) {
        Renderer::InputDispatcher::getInstance().dispatchMouseMove(xPos, yPos);
    });

    glfwSetScrollCallback(window, [](GLFWwindow*, const double xOffset, const double yOffset) {
        Renderer::InputDispatcher::getInstance().dispatchScroll(xOffset, yOffset);
    });

    /* Loop until the user closes the window */
    do {
        sceneManager->render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window) &&
             glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);

    sceneManager->onDestroy();

    glfwTerminate();
    return 0;
}
