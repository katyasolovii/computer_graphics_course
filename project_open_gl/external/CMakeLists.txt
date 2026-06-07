
add_subdirectory(glfw)

add_subdirectory(glad)

### ASSIMP ###
add_subdirectory(assimp-6.0.1)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)
