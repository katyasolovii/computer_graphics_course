
set(RENDERER_TARGET renderer)

find_package(OpenGL REQUIRED)

set(OpenGL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        glad
        glm
        assimp
)

add_subdirectory(external)

set(UTILS_DIR utils)
file(GLOB_RECURSE UTILS_SRC
        ${UTILS_DIR}/*.cpp
        ${UTILS_DIR}/*.h
)
MESSAGE("UTILS: " ${UTILS_SRC})

set(RENDERER_DIR renderer)
file(GLOB_RECURSE RENDERER_SRC
        ${RENDERER_DIR}/*.h
        ${RENDERER_DIR}/*.cpp
)
MESSAGE("Renderer: " ${RENDERER_SRC})

add_library(${RENDERER_TARGET} STATIC
        ${RENDERER_SRC}
        ${UTILS_SRC}
)
target_link_libraries(${RENDERER_TARGET} ${OpenGL_LIBS})
target_include_directories(${RENDERER_TARGET} PUBLIC ${UTILS_DIR} )
target_include_directories(${RENDERER_TARGET} PUBLIC ${RENDERER_DIR})

