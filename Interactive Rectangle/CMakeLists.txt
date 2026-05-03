cmake_minimum_required(VERSION 3.30)
project(Interactive_Rectangle)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(external)

find_package(OpenGL REQUIRED)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        glad
)

add_executable(Interactive_Task
        scr/main.cpp
        utils/shader_utils.cpp
        utils/shader_utils.h
        utils/texture.cpp
        utils/texture.h
        utils/stb_image.h
)

target_include_directories(Interactive_Task PRIVATE utils)
target_include_directories(Interactive_Task PRIVATE external/glm)
target_include_directories(Interactive_Task PRIVATE external/glad/include)
target_include_directories(Interactive_Task PRIVATE external/glfw/include)
target_link_libraries(Interactive_Task ${ALL_LIBS})

add_custom_command(
        TARGET Interactive_Task POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:Interactive_Task>/res
)