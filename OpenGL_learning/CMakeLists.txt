cmake_minimum_required(VERSION 3.30)
project(OpenGL_learning)

set(CMAKE_CXX_STANDARD 17)

# Додаємо бібліотеки (glfw, glad, glm)
add_subdirectory(external)

find_package(OpenGL REQUIRED)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        glad
        glm
)

# додаємо main.cpp та shader_utils, щоб функції LoadShaderFromFile були доступні.
add_executable(OpenGL_learning 
    src/main.cpp 
    src/shader_utils.cpp 
    src/shader_utils.h
)

target_link_libraries(OpenGL_learning ${ALL_LIBS})


add_custom_command(
        TARGET OpenGL_learning POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/res
        $<TARGET_FILE_DIR:OpenGL_learning>/res
)