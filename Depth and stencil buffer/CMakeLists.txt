cmake_minimum_required(VERSION 3.10)
project(ComputerGraphics_Task)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external)

find_package(OpenGL REQUIRED)

set(libs
        ${OPENGL_LIBRARY}
        glfw
        glad
        glm
)

set(UTILS_DIR utils)
file(GLOB_RECURSE UTILS_SRC
        ${UTILS_DIR}/*.cpp
        ${UTILS_DIR}/*.h
)

add_executable(CubeTask scr/main.cpp ${UTILS_SRC})

target_include_directories(CubeTask PRIVATE ${UTILS_DIR})
target_include_directories(CubeTask PRIVATE external/glm)
target_include_directories(CubeTask PRIVATE external/glad/include)
target_include_directories(CubeTask PRIVATE external/glfw/include)

target_link_libraries(CubeTask ${libs})

add_custom_command(
        TARGET CubeTask POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:CubeTask>/res
)