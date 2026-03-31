cmake_minimum_required(VERSION 3.30)
project(OpenGL_learning)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        glad
        glm
)

add_subdirectory(external)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})