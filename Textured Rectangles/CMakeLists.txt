cmake_minimum_required(VERSION 3.30)
project(Textured_Rectangles)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external)

find_package(OpenGL REQUIRED)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        glad
)

add_executable(OpenGL_learning
        scr/main.cpp
        utils/shader_utils.cpp
        utils/shader_utils.h
        utils/texture.cpp
        utils/texture.h
        utils/stb_image.h
)


target_include_directories(OpenGL_learning PRIVATE external/glm)
target_include_directories(OpenGL_learning PRIVATE external/glad/include)
target_include_directories(OpenGL_learning PRIVATE external/glfw/include)

target_link_libraries(OpenGL_learning ${ALL_LIBS})
add_custom_command(
        TARGET OpenGL_learning POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:OpenGL_learning>/res
)